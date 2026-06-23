# **********************************************
# Application Makefile Config
# **********************************************
TARGET := test
LOCAL_INCS := . ./include
LOCAL_SRCS := .
CC ?= gcc

ARCH_CFLAGS := -m32
LOCAL_CFLAGS :=

# **********************************************
# Common CFLAGS
# **********************************************
# LOCAL_CFLAGS += -Wno-multichar -fno-exceptions -ffunction-sections -fdata-sections -funwind-tables \
# 			-fno-short-enums -fno-builtin-sin -fno-strict-volatile-bitfields -fmessage-length=0 \
# 			-fgcse-after-reload -frerun-cse-after-loop -frename-registers -fomit-frame-pointer \
# 			-fno-strict-aliasing -flto

LOCAL_CFLAGS += -Wno-multichar -fno-exceptions -ffunction-sections -fdata-sections -funwind-tables \
			-fno-short-enums -fno-builtin-sin  -fmessage-length=0 \
			-fno-strict-aliasing

# **********************************************
# Common Makefile Rules (C Source Files)
# **********************************************
CFLAGS += $(ARCH_CFLAGS) $(LOCAL_CFLAGS) -Wall $(foreach dir, $(LOCAL_INCS), -I$(dir))

ifneq ($(wildcard config.h),)
	CFLAGS += -include config.h
endif

SRCS  := $(foreach file, $(foreach dir, $(LOCAL_SRCS), $(wildcard $(dir)/*.S)), $(subst ./, , $(file))) \
                   $(foreach file, $(foreach dir, $(LOCAL_SRCS), $(wildcard $(dir)/*.c)), $(subst ./, , $(file))) \
                   $(foreach file, $(foreach dir, $(LOCAL_SRCS), $(wildcard $(dir)/*.cpp)), $(subst ./, , $(file)))

OBJS := $(patsubst %.S,%.o, $(patsubst %.c,%.o, $(patsubst %.cpp, %.o, $(SRCS))))

.PHONY: all clean

all: $(OBJS)

clean:
	rm -f $(OBJS)
