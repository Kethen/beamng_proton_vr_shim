IMAGE_TAG="ubuntu:24.04"

podman run --rm -it \
	-v ./:/work_dir \
	-w /work_dir \
	--entrypoint /bin/bash \
	$IMAGE_TAG \
	-c '
	apt update; apt install -y g++-mingw-w64 gcc-mingw-w64
	bash build.sh
'
