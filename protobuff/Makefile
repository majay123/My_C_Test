# **********************************************
# Application Makefile Config
# **********************************************
TARGET := protobuff

LOCAL_INCS := . ./include \
				../include
LOCAL_SRCS := .

CC ?= gcc
EXTR_CFLAGS:= $(shell protoc --c_out=. *.proto)

LOCAL_CFLAGS :=
LOCAL_LDFLAGS := -L./lib -Wl,-rpath,./lib -lprotobuf-c -lpthread -lm  
#附加C编译选项

# **********************************************
# Common CFLAGS & LDFLAGS
# **********************************************
LOCAL_CFLAGS += -Wno-multichar -fno-exceptions -ffunction-sections -fdata-sections -funwind-tables \
			-fno-short-enums -fno-builtin-sin -fno-strict-volatile-bitfields -fmessage-length=0 \
			-fgcse-after-reload -frerun-cse-after-loop -frename-registers -fomit-frame-pointer \
			-fno-strict-aliasing -flto  
LOCAL_LDFLAGS += -Wl,--gc-sections -Wl,--no-undefined

# $(CC) $(CFLAGS) $^  -o $@  ./lib/libtest.a

# **********************************************
# Common Makefile Rules (C Source Files)
# **********************************************
CFLAGS += $(LOCAL_CFLAGS) -Wall $(foreach dir, $(LOCAL_INCS), -I$(dir))
LDFLAGS += $(LOCAL_LDFLAGS)

ifeq ($(suffix $(TARGET)),.so)
	CFLAGS += -fPIC
	LDFLAGS += -shared
endif

ifneq ($(wildcard config.h),)
	CFLAGS += -include config.h
endif

SRCS  := $(foreach file, $(foreach dir, $(LOCAL_SRCS), $(wildcard $(dir)/*.S)), $(subst ./, , $(file))) \
                   $(foreach file, $(foreach dir, $(LOCAL_SRCS), $(wildcard $(dir)/*.c)), $(subst ./, , $(file))) \
                   $(foreach file, $(foreach dir, $(LOCAL_SRCS), $(wildcard $(dir)/*.cpp)), $(subst ./, , $(file)))

OBJS := $(patsubst %.S,%.o, $(patsubst %.c,%.o, $(patsubst %.cpp, %.o, $(SRCS))))

.PHONY : all clean

all : $(TARGET)

$(TARGET) : $(OBJS)
ifeq ($(suffix $(TARGET)),.a)
	$(AR) curv $@ $(OBJS)
	$(RANLIB) $(TARGET)
else
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)
endif

clean :
	rm -rf $(OBJS)
	rm -rf $(TARGET)
	# rm -rf *.pb-c.*

