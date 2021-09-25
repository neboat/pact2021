CC = clang
CXX = clang++
LLVM_CONFIG = llvm-config

TARGETS = nqueens seq-nqueens toy-spawn-sync toy-parfor
RUNTIME_BUILD = /tutorial/cheetah/build

.PHONY: clean cheetah

# Set flags for different OpenCilk tools
ifeq ($(CILKSAN),1)
  CFLAGS = -fsanitize=cilk -Og -g
else
  CFLAGS = -O3
ifeq ($(CILKSCALE),1)
  CFLAGS += -fcilktool=cilkscale
endif
endif

default: $(TARGETS)

% : %.c
	$(CC) -g $< `$(LLVM_CONFIG) --cflags --ldflags --system-libs --libs core orcjit native` -O3 -o $@

% : %.cpp
	$(CXX) -g $< `$(LLVM_CONFIG) --cxxflags --ldflags --system-libs --libs core orcjit native` -O3 -o $@

nqueens : nqueens.c
	$(CC) -femulated-tls -g $< -fopencilk --opencilk-resource-dir=/tutorial/cheetah/build $(CFLAGS) -o $@

# run:
# 	LD_LIBRARY_PATH="$(PWD)/cheetah/build/lib/`$(LLVM_CONFIG) --host-target`/:`$(LLVM_CONFIG) --libdir`/clang/12.0.0/lib/`$(LLVM_CONFIG) --host-target`/" LIBRARY_PATH=$$LD_LIBRARY_PATH ./$(TARGET) $(OPTS)

cheetah:
	mkdir -p $(RUNTIME_BUILD)
	cmake --build $(RUNTIME_BUILD)

clean:
	cmake --build $(RUNTIME_BUILD) --target clean
	rm -rf $(TARGETS) *~
