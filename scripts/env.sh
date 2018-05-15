if [ $(uname -o) == "Msys" ] ; then
  sh scripts/docker.sh "$@"
  exit
fi

cd zephyr
. ./zephyr-env.sh
cd ..

if [ X$ZEPHYR_TOOLCHAIN_VARIANT = X ] ; then
  export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
  export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk
fi
