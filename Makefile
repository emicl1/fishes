CC = /usr/bin/arm-linux-gnueabihf-gcc
CXX = /usr/bin/arm-linux-gnueabihf-g++

CPPFLAGS = -I .
CFLAGS =-g -std=gnu99 -O1 -Wall
CXXFLAGS = -g -std=gnu++11 -O1 -Wall
#LDFLAGS +=
LDFLAGS += -static
LDLIBS += -lrt -lpthread
#LDLIBS += -lm

SOURCES = bigger_fish.cpp mzapo_phys.cpp mzapo_parlcd.cpp serialize_lock.cpp
#SOURCES += font_prop14x16.c font_rom8x16.c
TARGET_EXE = bigger_fish
TARGET_IP ?= 192.168.223.163
ifeq ($(TARGET_IP),)
ifneq ($(filter debug run,$(MAKECMDGOALS)),)
$(warning The target IP address is not set)
$(warning Run as "TARGET_IP=192.168.202.163 make run" or modify Makefile)
TARGET_IP ?= 192.168.223.163
endif
endif
TARGET_DIR ?= /tmp/$(shell whoami)
TARGET_USER ?= root
# for use from Eduroam network use TARGET_IP=localhost and enable next line
#SSH_OPTIONS=-o "UserKnownHostsFile=/dev/null" -o "StrictHostKeyChecking=no" -o "Port=2222"
#SSH_GDB_TUNNEL_REQUIRED=y
SSH_OPTIONS+=-i ~/.ssh/mzapo-root-key
SSH_OPTIONS+=-o 'ProxyJump=michaal4@postel.felk.cvut.cz'

OBJECTS += $(filter %.o,$(SOURCES:%.c=%.o))
OBJECTS += $(filter %.o,$(SOURCES:%.cpp=%.o))

#$(warning OBJECTS=$(OBJECTS))

ifeq ($(filter %.cpp,$(SOURCES)),)
LINKER = $(CC)
LDFLAGS += $(CFLAGS) $(CPPFLAGS)
else
LINKER = $(CXX)
LDFLAGS += $(CXXFLAGS) $(CPPFLAGS)
endif

%.o:%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJECTS)
	$(LINKER) $(LDFLAGS) -L. $^ -o $@ $(LDLIBS)

.PHONY : dep all run copy-executable debug

dep: depend

depend: $(SOURCES) *.h
	echo '# autogenerated dependencies' > depend
ifneq ($(filter %.c,$(SOURCES)),)
	$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M $(filter %.c,$(SOURCES)) \
	  >> depend
endif
ifneq ($(filter %.cpp,$(SOURCES)),)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -w -E -M $(filter %.cpp,$(SOURCES)) \
	  >> depend
endif

clean:
	rm -f *.o *.a $(OBJECTS) $(TARGET_EXE) connect.gdb depend

copy-executable: $(TARGET_EXE)
	ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) killall gdbserver 1>/dev/null 2>/dev/null || true
	ssh $(SSH_OPTIONS) $(TARGET_USER)@$(TARGET_IP) mkdir -p $(TARGET_DIR)
	scp $(SSH_OPTIONS) $(TARGET_EXE) $(TARGET_USER)@$(TARGET_IP):$(TARGET_DIR)/$(TARGET_EXE)

run: copy-executable $(TARGET_EXE)
	ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) $(TARGET_DIR)/$(TARGET_EXE)

ifneq ($(filter -o ProxyJump=,$(SSH_OPTIONS))$(SSH_GDB_TUNNEL_REQUIRED),)
SSH_GDB_PORT_FORWARD=-L 12345:127.0.0.1:12345
TARGET_GDB_PORT=127.0.0.1:12345
else
TARGET_GDB_PORT=$(TARGET_IP):12345
endif

debug: copy-executable $(TARGET_EXE)
	xterm -e ssh $(SSH_OPTIONS) $(SSH_GDB_PORT_FORWARD) -t $(TARGET_USER)@$(TARGET_IP) gdbserver :12345 $(TARGET_DIR)/$(TARGET_EXE) &
	sleep 2
	echo >connect.gdb "target extended-remote $(TARGET_GDB_PORT)"
	echo >>connect.gdb "b main"
	echo >>connect.gdb "c"
	ddd --debugger gdb-multiarch -x connect.gdb $(TARGET_EXE)

-include depend
