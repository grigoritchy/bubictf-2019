# Origin image
FROM ubuntu:16.04

# Meta Information
MAINTAINER grigoritchy "grigoritchy@gmail.com"

# update
RUN apt update -y

RUN apt upgrade -y

# Setup Server Environment
RUN apt install -y \
    git \
    python2.7 \
    socat \
    make \
    cmake

# add new user if it is needed
RUN useradd -d /home/ctf/ -m -p ctf -s /bin/bash ctf
RUN echo "ctf:ctf" | chpasswd

# Change work directory
WORKDIR /home/ctf

# Setup the vulnerability environment
COPY source/ .

RUN chmod +x ./run.py
RUN ln -s /usr/bin/python2.7 /usr/bin/python
RUN chmod 555 /home/ctf
RUN chmod 770 /tmp

# Change user
USER ctf

# Entry point
ENTRYPOINT socat tcp-l:5555,fork,reuseaddr exec:./run.py && /bin/bash
