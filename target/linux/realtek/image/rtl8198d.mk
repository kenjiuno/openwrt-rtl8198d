# SPDX-License-Identifier: GPL-2.0-only

include ./common.mk

define Device/elecom_wrc-x1800gs2
  SOC := rtl8198d
  IMAGE_SIZE := 3968k
  DEVICE_VENDOR := ELECOM
  DEVICE_MODEL := WRC-X1800GS2 (TODO: Login via ttyS0, Eth, LEDs, GPIO, RTL8192XBR, RTL8832CR, WLAN)
  UIMAGE_MAGIC := 0x27051956
  UIMAGE_NAME := Linux-5.10.70
endef
TARGET_DEVICES += elecom_wrc-x1800gs2
