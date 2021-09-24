CC = clang
CXX = clang++
LLVM_CONFIG = llvm-config

TARGETS = nqueens seq-nqueens toy-spawn-sync toy-parfor

# Set flags for different OpenCilk tools
ifeq ($(CILKSAN),1)
  CFLAGS = -fsanitize=cilk -Og -g
else
  CFLAGS = -O3
ifeq ($(CILKSCALE),1)
  CFLAGS += -fcilktool=cilkscale
endif
endif

default: $(TARGET)

% : %.c
	$(CC) -g $< `$(LLVM_CONFIG) --cflags --ldflags --system-libs --libs core orcjit native` -O3 -o $@

% : %.cpp
	$(CXX) -g $< `$(LLVM_CONFIG) --cxxflags --ldflags --system-libs --libs core orcjit native` -O3 -o $@

nqueens : nqueens.c
	$(CC) -femulated-tls -g $< -fopencilk --opencilk-resource-dir=/tutorial/cheetah/build $(CFLAGS) -o $@

# run:
# 	LD_LIBRARY_PATH="$(PWD)/cheetah/build/lib/`$(LLVM_CONFIG) --host-target`/:`$(LLVM_CONFIG) --libdir`/clang/12.0.0/lib/`$(LLVM_CONFIG) --host-target`/" LIBRARY_PATH=$$LD_LIBRARY_PATH ./$(TARGET) $(OPTS)

clean:
	rm -rf $(TARGETS) *~
