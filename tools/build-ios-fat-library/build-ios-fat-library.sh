# Used for build library for iOS (simulator & device)
# ©2015-2016 Frank Lin

FILE_NAME=libsakura.a

echo "Building fat library : ${FILE_NAME}"

RELEASE_DEVICE_DIR="${SYMROOT}/Release-iphoneos"
RELEASE_SIMULATOR_DIR="${SYMROOT}/Release-iphonesimulator"
RELEASE_UNIVERSAL_DIR="${SYMROOT}/Release-iphoneuniversal"

rm -rf "${RELEASE_UNIVERSAL_DIR}"
mkdir "${RELEASE_UNIVERSAL_DIR}"

lipo -create "${RELEASE_DEVICE_DIR}/${FILE_NAME}" "${RELEASE_SIMULATOR_DIR}/${FILE_NAME}" -output "${RELEASE_UNIVERSAL_DIR}/${FILE_NAME}"
