# Use an official Ubuntu base image
FROM ubuntu:latest

# Set environment variables to prevent interactive prompts during package installations
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary dependencies for building and running your project (gcc, make, libraries)
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    libc6 \
    libgcc1 \
    libstdc++6 \
    && apt-get clean

# Set the working directory inside the container
WORKDIR /app

# Copy the entire project folder into the container
COPY . /app

# Clean previous builds, build the project using `make`
RUN make clean && make

# By default, if no argument is passed to `docker run`, `make` will execute
CMD ["make", "run_vamana_main"]

RUN apt-get update && apt-get install -y valgrind
