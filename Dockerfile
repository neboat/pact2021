FROM ubuntu:20.04

# NOTE: Don't use ADD; ADD will decompress the file
COPY opencilk.tar.gz /usr/local/src/opencilk.tar.gz
COPY tutorial.tar.gz /tutorial.tar.gz

# Note that it is possible to specify a sequence of RUN commands, but there
# are various issues with this practice. The first issue is that each separate
# RUN command creates an extra layer of information that is stored with the
# container, which significantly bloats the image. Essentially these layers are
# diffs, so if we extract the opencilk tar in a step separate from removing the
# extracted files then Docker still stores those files in one of the layers, thus
# causing the Docker image to increase in size.

# Step 1: Install build dependencies (TODO: may be possible to install less)
# Step 2: Untar, build, and install OpenCilk
# Step 3: Remove the build files and apt-get cache files
RUN echo "Installing packages..." \
  && apt-get update -qq > /dev/null \
  && apt-get install -qqy --no-install-recommends \
    cmake \
    clang \
    lld \
    make \
    python3-minimal \
    vim emacs \
    > /dev/null \
  && echo "Building OpenCilk..." \
  && tar -C /usr/local/src -xzf /usr/local/src/opencilk.tar.gz \
  && mkdir /tutorial \
  && tar -C / -xzf /tutorial.tar.gz \
  && mkdir -p /usr/local/src/opencilk/build \
  && /usr/local/src/opencilk/infrastructure/tools/build /usr/local/src/opencilk/opencilk-project \
    /usr/local/src/opencilk/build \
  && echo "Installing OpenCilk to default install location..." \
  && cmake --build /usr/local/src/opencilk/build --target install \
  && echo "Cleaning temporary files..." \
  && rm -r /usr/local/src/opencilk \
  && rm -rf /var/lib/apt/lists/* \
  && mkdir -p /tutorial/cheetah/build  && cd /tutorial/cheetah/build \
  && cmake -DCMAKE_C_COMPILER=clang -DCMAKE_C_FLAGS="-O3 -g -femulated-tls" -DCHEETAH_ENABLE_ASAN=OFF .. \
  && cmake --build . \
  && echo "export LIBRARY_PATH=/tutorial/cheetah/build/lib/x86_64-unknown-linux-gnu" >> /root/.bashrc \
  && echo "export LD_LIBRARY_PATH=$LIBRARY_PATH" >> /root/.bashrc \
  && echo "DOCKER IMAGE BUILT"
