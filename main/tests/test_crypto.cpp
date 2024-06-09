/**************************************************************************/
/*  test_crypto.cpp                                                       */
/**************************************************************************/


#include "core/crypto/crypto.h"
#include "core/os/os.h"

namespace TestCrypto {

class _MockCrypto : public Crypto {
	virtual PoolByteArray generate_random_bytes(int p_bytes) { return PoolByteArray(); }
	virtual Ref<CryptoKey> generate_rsa(int p_bytes) { return nullptr; }
	virtual Ref<X509Certificate> generate_self_signed_certificate(Ref<CryptoKey> p_key, String p_issuer_name, String p_not_before, String p_not_after) { return nullptr; }

	virtual Vector<uint8_t> encrypt(Ref<CryptoKey> p_key, Vector<uint8_t> p_plaintext) { return Vector<uint8_t>(); }
	virtual Vector<uint8_t> decrypt(Ref<CryptoKey> p_key, Vector<uint8_t> p_ciphertext) { return Vector<uint8_t>(); }
};

PoolByteArray raw_to_pba(const uint8_t *arr, size_t len) {
	PoolByteArray pba;
	pba.resize(len);
	for (size_t i = 0; i < len; i++) {
		pba.set(i, arr[i]);
	}
	return pba;
}

typedef bool (*TestFunc)();

TestFunc test_funcs[] = {
	nullptr
};

MainLoop *test() {
	int count = 0;
	int passed = 0;

	while (true) {
		if (!test_funcs[count]) {
			break;
		}
		bool pass = test_funcs[count]();
		if (pass) {
			passed++;
		}
		OS::get_singleton()->print("\t%s\n", pass ? "PASS" : "FAILED");

		count++;
	}
	OS::get_singleton()->print("\n");
	OS::get_singleton()->print("Passed %i of %i tests\n", passed, count);
	return nullptr;
}

} // namespace TestCrypto
