#
# Copyright 2008-2019 Ettus Research, a National Instruments Brand
#

##################################################
# Project Setup
##################################################
# TOP_MODULE = <Input arg>
# NAME = <Input arg>
# PART_ID = <Input arg>
# ARCH = <Input arg>

##################################################
# Include other makefiles
##################################################

BASE_DIR = $(abspath ..)
IP_DIR = $(abspath ./ip)
include $(BASE_DIR)/../tools/make/viv_design_builder.mak

include $(IP_DIR)/Makefile.inc
include coregen_dsp/Makefile.srcs
include $(LIB_DIR)/ip/Makefile.inc
include $(LIB_DIR)/hls/Makefile.inc
include $(LIB_DIR)/control/Makefile.srcs
include $(LIB_DIR)/fifo/Makefile.srcs
include $(LIB_DIR)/simple_gemac/Makefile.srcs
include $(LIB_DIR)/axi/Makefile.srcs
include $(LIB_DIR)/timing/Makefile.srcs
include $(LIB_DIR)/packet_proc/Makefile.srcs
include $(LIB_DIR)/xge/Makefile.srcs
include $(LIB_DIR)/xge_interface/Makefile.srcs
include $(LIB_DIR)/dsp/Makefile.srcs
include $(LIB_DIR)/axi/Makefile.srcs
include $(LIB_DIR)/white_rabbit/wr_cores_v4_2/Makefile.srcs
include $(LIB_DIR)/rfnoc/Makefile.srcs
include $(BASE_DIR)/n3xx/dboards/rh/Makefile.srcs
include $(BASE_DIR)/n3xx/dboards/mg/Makefile.srcs
include $(BASE_DIR)/n3xx/dboards/common/Makefile.srcs
include $(LIB_DIR)/axi4s_sv/Makefile.srcs
include $(LIB_DIR)/rfnoc/xport_sv/Makefile.srcs

# If out-of-tree modules want to be compiled into this image, then they need to
# pass in the RFNOC_OOT_MAKEFILE_SRCS as a list of Makefile.srcs files.
# Those files need to amend the RFNOC_OOT_SRCS variable with a list of actual
# source files.
-include $(RFNOC_OOT_MAKEFILE_SRCS)

ifdef BUILD_DIR
include $(BUILD_DIR)/Makefile.inc
endif

##################################################
# Sources
##################################################
TOP_SRCS = \
n3xx_core.v \
n3xx_mgt_channel_wrapper.v \
n3xx_mgt_wrapper.v \
n3xx_mgt_io_core.v \
n3xx_clocking.v \
n3xx_db_fe_core.v \
WrapBufg.vhd \
$(IMAGE_CORE)

MB_CLOCKS_XDC = $(abspath mb_clocks.xdc)

MB_XDC = \
mb_timing.xdc \
mb_pins.xdc

ifdef BUILD_WR
MB_XDC += $(abspath n3xx_wr.xdc)
TOP_SRCS += \
n3xx_wr_top.vhd \
n3xx_serial_dac.vhd \
n3xx_serial_dac_arb.vhd
endif

ifdef BUILD_10G
MB_XDC += $(abspath n310_10ge.xdc)
endif

ifdef BUILD_1G
MB_XDC += $(abspath n310_1ge.xdc)
endif

ifdef BUILD_AURORA
MB_XDC += $(abspath n310_aurora.xdc)
endif

ifndef NO_DRAM_FIFOS
DRAM_SRCS = $(IP_DRAM_XCI_SRCS) $(abspath n310_dram.xdc)
else
DRAM_SRCS =
endif

ifdef N300
DB_CLOCKS_XDC =
DB_SRCS = $(MAGNESIUM_DB_TIMING_XDC) \
$(MAGNESIUM_DB0_XDC) \
$(MAGNESIUM_TOP_SRCS) $(MAGNESIUM_DB_SRCS)
endif

ifdef N310
DB_CLOCKS_XDC =
DB_SRCS = $(MAGNESIUM_DB_TIMING_XDC) \
$(MAGNESIUM_DB0_XDC) $(MAGNESIUM_DB1_XDC) \
$(MAGNESIUM_TOP_SRCS) $(MAGNESIUM_DB_SRCS)
endif

ifdef N320
DB_CLOCKS_XDC = $(RHODIUM_DB_CLOCKS_XDC)
DB_SRCS = $(RHODIUM_DB_XDC) \
$(RHODIUM_TOP_SRCS) $(RHODIUM_DB_SRCS)
endif

DB_SRCS += $(DB_COMMON_SRCS)

# The XDC files must be read in a specific order, motherboard first and then
# daughterboard, and the clocking XDC files must be before other XDC files.
DESIGN_SRCS = $(abspath \
$(AXI_SRCS) $(AXI4S_SV_SRCS) $(FIFO_SRCS) \
$(CONTROL_LIB_SRCS) $(SDR_LIB_SRCS) $(SERDES_SRCS) \
$(SIMPLE_GEMAC_SRCS) $(TIMING_SRCS) $(COREGEN_DSP_SRCS) \
$(UDP_SRCS) $(EXTRAM_SRCS) $(WISHBONE_SRCS) \
$(XGE_SRCS) $(XGE_INTERFACE_SRCS) $(AURORA_PHY_SRCS) \
$(TEN_GIGE_PHY_SRCS) $(ONE_GIGE_PHY_SRCS) \
$(PACKET_PROC_SRCS) $(DSP_SRCS) $(DRAM_SRCS) \
$(RADIO_SRCS) $(CAP_GEN_GENERIC_SRCS) $(IP_XCI_SRCS) $(BD_SRCS) \
$(RFNOC_FRAMEWORK_SRCS) \
$(RFNOC_SRCS) $(RFNOC_OOT_SRCS) $(LIB_IP_XCI_SRCS) $(LIB_HLS_IP_SRCS) \
$(WHITE_RABBIT_SRCS) \
$(RFNOC_XPORT_SV_SRCS) \
$(RFNOC_IMAGE_CORE_SRCS) \
$(MB_CLOCKS_XDC) $(DB_CLOCKS_XDC) $(MB_XDC) $(DB_SRCS) $(TOP_SRCS) \
)

# Pass the image core header files required by RFNoC
# to Vivado as Verilog definitions.
IMAGE_CORE_HEADER_DEF="RFNOC_IMAGE_CORE_HDR=$(BUILD_DIR)/rfnoc_image_core.vh"

##################################################
# Dependency Targets
##################################################
.SECONDEXPANSION:

VERILOG_DEFS=$(EXTRA_DEFS) $(CUSTOM_DEFS) $(GIT_HASH_VERILOG_DEF) $(IMAGE_CORE_HEADER_DEF) $(RFNOC_IMAGE_CORE_DEFS)

# DESIGN_SRCS and VERILOG_DEFS must be defined
bin: .prereqs
	@echo "Build directory:: $(BUILD_DIR)"
	@echo "Printing MB_XDC:: $(MB_XDC)"
	@echo "Printing VERILOG_DEFS:: $(VERILOG_DEFS)"
	$(call BUILD_VIVADO_DESIGN,$(abspath ./build_n3xx.tcl),$(TOP_MODULE),$(ARCH),$(PART_ID))

synth: .prereqs
	$(call BUILD_VIVADO_DESIGN,$(TOOLS_DIR)/scripts/viv_synth.tcl,$(TOP_MODULE),$(ARCH),$(PART_ID))

rtl: .prereqs
	$(call CHECK_VIVADO_DESIGN,$(TOOLS_DIR)/scripts/viv_check_syntax.tcl,$(TOP_MODULE),$(ARCH),$(PART_ID))

viv_ip: .prereqs $$(DESIGN_SRCS) ip
	@echo "IP build for $(NAME) DONE . . ."

.PHONY: bin synth rtl viv_ip
