CXX = g++
CXXFLAGS = -g -Wall
SOURCES = src/runner.cc \
	src/event_dispatcher.cc \
	src/dummy_server_socket.cc \
	test/basic_sample.cc
OBJS = $(SOURCES:%.cc=%.o)

# output files
TARGET = basic_sample
DEPEND_FILE = depend_file

.SUFFIXES = .cc .o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -o $@ $(OBJS)

$(OBJS) : $(SOURCES)

$(SOURCES) :
	$(CXX) $(CXXFLAGS) $(SOURCES)

depend :
	$(CXX) -MM $(SOURCES) > $(DEPEND_FILE)

clean :
	rm $(OBJS) $(TARGET) $(DEPEND_FILE)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(SOURCES),)
-include $(DEPEND_FILE)
endif
endif
endif
