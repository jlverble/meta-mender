
# softhsm opensc gnutls p11-kit libp11 are needed for MEN-3905
# to access the private key from HSM (emulated via SoftHSM2)
# * softhsm: SoftHSM2, a software emulation of a hardware security module
# * gnutls: required for p11tool for accessing the key for testing, depends on p11-kit
# * libp11: OpenSSL ENGINE plugin PKCS#11 implementation, depends on p11-kit
#           for the OpenSSL to load the lib and access the key from the HSM
# * opensc: pkcs11-tool for initialization of token
# * gnutls-bin: provides p11tool for getting the contents of the token
DEPENDS:append:mender-image:qemuall = " softhsm opensc gnutls p11-kit libp11"
RDEPENDS:${PN}:append:mender-image:qemuall = " softhsm opensc gnutls p11-kit libp11 gnutls-bin"

# Needed for the TestFaultTolerance tests in the integration repository.
RDEPENDS:${PN}:append:mender-image:qemuall = " \
    kernel-module-ipt-reject \
    kernel-module-ts-bm \
    kernel-module-xt-string \
"

# Used as a rootfs-image replacement when testing update modules.
FILES:${PN}:append:mender-image:qemuall = " ${sysconfdir}/mender/rootfs-image-v2.conf \
                                            ${datadir}/mender/modules/v3/rootfs-image-v2"

do_compile:append:mender-image:qemuall() {
    if ${@bb.utils.contains('PACKAGECONFIG', 'modules', 'true', 'false', d)}; then
        cat > ${WORKDIR}/rootfs-image-v2.conf <<EOF
MENDER_ROOTFS_PART_A="${MENDER_ROOTFS_PART_A}"
MENDER_ROOTFS_PART_B="${MENDER_ROOTFS_PART_B}"
EOF
    fi
}

do_install:append:mender-image:qemuall() {
    if ${@bb.utils.contains('PACKAGECONFIG', 'modules', 'true', 'false', d)}; then
        install -m 755 -d ${D}${datadir}/mender/modules/v3
        install -m 755 ${B}/src/${GO_IMPORT}/tests/rootfs-image-v2 ${D}${datadir}/mender/modules/v3/

        install -m 755 -d ${D}${sysconfdir}/mender
        install -m 644 ${WORKDIR}/rootfs-image-v2.conf ${D}${sysconfdir}/mender/
    fi
}
