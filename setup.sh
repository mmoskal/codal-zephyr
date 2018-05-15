ME=setup.sh
if [ "X$(basename -- "$0")" "==" "X$ME" ]; then
    echo "Run '. $ME' not './$ME'"
    exit
fi

cd zephyr
. zephyr-env.sh
cd ..

(
set -x
set -e
rm -rf build
mkdir build
cd build
cmake -DBOARD=nucleo_f411re ../app
)
