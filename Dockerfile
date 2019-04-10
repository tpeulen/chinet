FROM ubuntu:latest
MAINTAINER Thomas-Otavio Peulen <tpeulen@salilab.org>
RUN wget https://repo.continuum.io/miniconda/Miniconda3-4.5.12-Linux-x86_64.sh && echo "e5e5b4cd2a918e0e96b395534222773f7241dc59d776db1b9f7fedfcb489157a  Miniconda3-4.5.12-Linux-x86_64.sh" | sha256sum -c --status && bash ./Miniconda3-4.5.12-Linux-x86_64.sh -b -p /root/miniconda && rm -f Miniconda3-4.5.12-Linux-x86_64.sh && echo "export PATH=\"/root/miniconda/bin:\$PATH\"" >> /root/.bashrc
RUN source /root/.bashrc && conda update -y --all && conda install -y conda-build curl git expat openssl && conda config --add channels tpeulen && conda config --add channels conda-forge
