// SPDX-License-Identifier: GPL-2.0-only

#include <asm/cacheflush.h>
#include <asm/mipsmtregs.h>
#include <dt-bindings/clock/rtl83xx-clk.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>
#include <linux/cpu.h>
#include <linux/delay.h>
#include <linux/genalloc.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#define read_soc(reg) ioread32(((void *)RTL_SOC_BASE) + reg)

#define RTL_SOC_BASE (0xB8000000)

#define RTL_oc0_cmugcr (0x0380)
#define RTL_ocp_pll_ctrl0 (0x0200)
#define RTL_ocp_pll_ctrl3 (0x020c)

static clk_hw clks[2];
static clk_onecell_data clk_data;

void pll_ocp_freq_mhz(void)
{
	uint clk;

	printfk("ocp_pll_ctrl0: %08X\n", (unsigned)read_soc(RTL_ocp_pll_ctrl0));
	printfk("ocp_pll_ctrl3: %08X\n", (unsigned)read_soc(RTL_ocp_pll_ctrl3));
	printfk("oc0_cmugcr: %08X\n", (unsigned)read_soc(RTL_oc0_cmugcr));

	clk = ((read_soc(RTL_ocp_pll_ctrl0) << 10) >> 0x1a) * 0x32 + 100 >> ((read_soc(RTL_ocp_pll_ctrl3) << 0xd) >> 0x1f);
	if ((read_soc(RTL_oc0_cmugcr) & 3) != 0) {
		clk = clk >> ((read_soc(RTLoc0_cmugcr) << 0x19) >> 0x1d);
	}
	printk("CPU clock is %u\n", clk);
	return clk;
}

static unsigned int pll_sys_LX_freq_mhz(void)
{
	unsigned int reg_val, LX_freq;

	reg_val = REG32(0xBB01F054);
	reg_val &= (0xf);
	LX_freq = 1000/(reg_val + 5);

	return LX_freq;
}

static struct clk_hw *apro_clocks_init(struct device_node *node)
{
	u32 ocp, lx;

	ocp = pll_ocp_freq_mhz();
	clks[0] = clk_register_fixed_rate(NULL, "ocp", NULL, CLK_IS_ROOT, ocp * 1000000);
	if (IS_ERR(clks[0]))
	{
		printk("[%s %d] apro_clocks_init failed!!!!\n", __FUNC__, __LINE__);
	}
	__sync();
	__sync();
	__sync();
	lx = pll_sys_LX_freq_mhz();
	clks[1] = clk_register_fixed_rate(NULL, "LX", NULL, CLK_IS_ROOT, lx * 1000000);
	if (IS_ERR(clks[1]))
	{
		printk("[%s %d] apro_clocks_init failed!!!!\n", __FUNC__, __LINE__);
	}
  	clk_data.clks = clks;
  	clk_data.clk_num = 2;
	of_clk_add_provider(node, of_clk_src_onecell_get, &clk_data);
	printk("[%s]clk_data.clk_num=%d, ocp=%u, lx=%u\n", __func__, clk_data.clk_num,
		ocp * 1000000, lx * 1000000);
	return clks;
}

CLK_OF_DECLARE(fixed_factor_clk, "fixed-factor-clock", of_fixed_factor_clk_setup);
CLK_OF_DECLARE(fixed_clk, "fixed-clock", of_fixed_clk_setup);
CLK_OF_DECLARE(apro, "rtk,apro-ocpclk", apro_clocks_init);
