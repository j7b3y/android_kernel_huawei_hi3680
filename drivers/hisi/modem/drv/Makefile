include $(srctree)/drivers/hisi/modem/config/product/$(OBB_PRODUCT_NAME)/$(OBB_MODEM_CUST_CONFIG_DIR)/config/balong_product_config.mk
#
# CONFIG_HISI_BALONG_EXTRA_MODEM A+B A�� defconfig �Ƿ����è
# makefile_soc_modem    ��A+B��̬���룬A+B B�����
# makefile_extra_modem  A+B A�����


ifeq ($(strip $(CONFIG_HISI_BALONG_EXTRA_MODEM)),y)
-include $(srctree)/drivers/hisi/modem/drv/makefile_extra_modem.mk

else
include $(srctree)/drivers/hisi/modem/drv/makefile_soc_modem.mk

endif

