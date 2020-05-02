PRJ_TOP_DIR = ../
CUR_DIR = $(shell pwd)
BUILDDIR = .obj
TARGETDIR = $(shell pwd)
COMPILER = g++

SRC_DIR = $(PRJ_TOP_DIR)/src/
INC_DIR = $(PRJ_TOP_DIR)/include/

# compiler para
CFLAGS += -std=c++11
LDFLAGS += -L$(PRJ_TOP_DIR)/lib -lpthread -lsqlite3

#common 
COMMON_SRC = sqlite.cpp socket.cpp untils.cpp blacklist.cpp
COMMON_INC_DIR = $(INC_DIR)/common/

COMMON_DIR = ./common/
COMMON_BDIR = $(SRC_DIR)$(COMMON_DIR)$(BUILDDIR)
COMMON_OBJ = $(foreach file, $(COMMON_SRC), $(COMMON_BDIR)/$(file:.cpp=.o))

#common 
LOG_SRC = logCtrl.cpp logManage.cpp
LOG_INC_DIR = $(INC_DIR)/logCtrl/

LOG_DIR = ./logCtrl/
LOG_BDIR = $(SRC_DIR)$(LOG_DIR)$(BUILDDIR)
LOG_OBJ = $(foreach file, $(LOG_SRC), $(LOG_BDIR)/$(file:.cpp=.o))

# delta note
DELTANOTE_APP = $(TARGETDIR)/deltaNote_server
DELTANOTE_SRC = deltaNote.cpp sqliteCtrl.cpp connectCtrl.cpp packCtrl.cpp
DELTANOTE_INC_DIR = $(INC_DIR)/deltaNote/

DELTANOTE_DIR = ./deltaNote/
DELTANOTE_BDIR = $(SRC_DIR)$(DELTANOTE_DIR)$(BUILDDIR)
DELTANOTE_OBJ = $(foreach file, $(DELTANOTE_SRC), $(DELTANOTE_BDIR)/$(file:.cpp=.o))

# sqlite tools
SQLITE_TOOLS_APP = $(TARGETDIR)/sqliteTools
SQLITE_TOOLS_SRC = sqliteTools.cpp cmdProcess.cpp sqliteCtrl.cpp
SQLITE_TOOLS_INC_DIR = $(INC_DIR)/sqliteTools/

SQLITE_TOOLS_DIR = ./sqliteTools/
SQLITE_TOOLS_BDIR = $(SRC_DIR)$(SQLITE_TOOLS_DIR)$(BUILDDIR)
SQLITE_TOOLS_OBJ = $(foreach file, $(SQLITE_TOOLS_SRC), $(SQLITE_TOOLS_BDIR)/$(file:.cpp=.o))

# common file
BDIRS = $(COMMON_BDIR) $(LOG_BDIR) $(DELTANOTE_BDIR) $(SQLITE_TOOLS_BDIR)
COMMON_INCL = -I$(LOG_INC_DIR) -I$(COMMON_INC_DIR) -I$(DELTANOTE_INC_DIR) -I$(SQLITE_TOOLS_INC_DIR)

# when no para default tartget
.PHONY : default_target
default_target : all

# gen all target
.PHONY : all
all : prepare $(DELTANOTE_APP) $(SQLITE_TOOLS_APP)

# target deltaNote_server
$(DELTANOTE_APP) : $(LOG_OBJ) $(COMMON_OBJ) $(DELTANOTE_OBJ) 
	$(COMPILER) $(CFLAGS) $(COMMON_INCL) $(LDFLAGS) -o $@ $(LOG_OBJ) $(COMMON_OBJ) $(DELTANOTE_OBJ)

$(DELTANOTE_OBJ) : $(DELTANOTE_BDIR)/%.o : $(SRC_DIR)/$(DELTANOTE_DIR)/%.cpp
	$(COMPILER) $(CFLAGS) $(COMMON_INCL) -c $< -o $@

#target sqliteTools
$(SQLITE_TOOLS_APP) : $(LOG_OBJ) $(COMMON_OBJ) $(SQLITE_TOOLS_OBJ) 
	$(COMPILER) $(CFLAGS) $(COMMON_INCL) $(LDFLAGS) -o $@ $(LOG_OBJ) $(COMMON_OBJ) $(SQLITE_TOOLS_OBJ) 

$(SQLITE_TOOLS_OBJ) : $(SQLITE_TOOLS_BDIR)/%.o : $(SRC_DIR)/$(SQLITE_TOOLS_DIR)/%.cpp
	$(COMPILER) $(CFLAGS) $(COMMON_INCL) -c $< -o $@

#log 
$(LOG_OBJ) : $(LOG_BDIR)/%.o : $(SRC_DIR)$(LOG_DIR)/%.cpp
	$(COMPILER) $(CFLAGS) $(COMMON_INCL) -c $< -o $@

#common 
$(COMMON_OBJ) : $(COMMON_BDIR)/%.o : $(SRC_DIR)$(COMMON_DIR)/%.cpp
	$(COMPILER) $(CFLAGS) $(COMMON_INCL) -c $< -o $@

.PHONY : clean prepare

prepare:
	@mkdir -p $(BDIRS)

clean :
	@rm -rvf $(BDIRS)
	@rm -rvf $(DELTANOTE_APP) $(SQLITE_TOOLS_APP)