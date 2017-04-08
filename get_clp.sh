mkdir -p external_code
cd external_code

wget http://www.coin-or.org/download/source/Clp/Clp-1.15.6.tgz
tar xvzf Clp-1.15.6.tgz
cd Clp-1.15.6/
./configure --enable-static; make install
