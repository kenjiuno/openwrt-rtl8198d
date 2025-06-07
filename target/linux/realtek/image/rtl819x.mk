# SPDX-License-Identifier: GPL-2.0-only

include ./common.mk

define Device/elecom_wrc-x1800gs2
  SOC := rtl8198
  IMAGE_SIZE := 7168k
  DEVICE_VENDOR := ELECOM
  DEVICE_MODEL := WRC-X1800GS2
  UIMAGE_MAGIC := 0x00000000
  UIMAGE_NAME := Linux Kernel Image
endef
TARGET_DEVICES += elecom_wrc-x1800gs2
