cleanflg=0
for I in $@; do
	if [ ${I} = "-clean" ]; then
		cleanflg=1
		break
	fi
done

if [ ! -e build ]; then
	echo "start clean build"
elif [ ${cleanflg} -eq 1 ]; then
	echo "start clean build"
	rm -rf build
fi

mkdir build
cd build

cmake ..
make
