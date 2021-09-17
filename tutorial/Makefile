CXX = clang++
LLVM_CONFIG = llvm-config

TARGETS = toy-spawn-sync toy-parfor

default: $(TARGET)

% : %.cpp
	$(CXX) -g $< `$(LLVM_CONFIG) --cxxflags --ldflags --system-libs --libs core orcjit native` -O3 -o $@

run:
	LD_LIBRARY_PATH="$(PWD)/cheetah/build/lib/`$(LLVM_CONFIG) --host-target`/:`$(LLVM_CONFIG) --libdir`/clang/12.0.0/lib/`$(LLVM_CONFIG) --host-target`/" LIBRARY_PATH=$$LD_LIBRARY_PATH ./$(TARGET) $(OPTS)

clean:
	rm -rf $(TARGETS)
