if [ $(uname -o) == "Msys" ] ; then
  sh scripts/docker.sh "$@"
  exit
fi

cd zephyr
. ./zephyr-env.sh
cd ..

export XDG_CACHE_HOME=`pwd`/build/homecache
