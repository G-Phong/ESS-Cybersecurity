/* Main source file for assignment 2 part A, which implements shitty homebrewn crypto together with
 * a nasty little stack based buffer overflow
 *
 * Copyright 2018 TUM
 * Created: 2018-11-15 Florian Wilde <florian.wilde@tum.de>
 **************************************************************************************************/

#include "VirtualSerial.h"
#include "packetizer.h"
#include "crypto.h"
#include "randombytes_salsa20XMC_random.h"
#include "sodium.h"

int main(void) {
  unsigned char key[32] = {};
  pt_s plaintext = { 0 };
  ct_s ciphertext = { 0 };
  plaintext.text = (unsigned char*) malloc(200);


  USB_Init();
  randombytes_set_implementation(&randombytes_salsa20XMC_implementation);
  sodium_init();
  printf("%i\n", a);
  while(1) {
    /* Consumes incoming data and parses it until we received a valid packet.
       If so, it will fill the plaintextJob struct with the appropriate information. */
    if(packetizerReceive(&plaintext) == 0) {

      //a = sodium_init();
      /* If the sender did not sent a nonce, generate one */
      if(plaintext.nonce == NULL) {
        plaintext.nonce = malloc(CRYPTO_NONCE_BYTES);
        for(size_t i = 0; i < CRYPTO_NONCE_BYTES; i++) {
          plaintext.nonce[i] = rand() & UINT8_MAX;
        }
      }

      ciphertext.textLen = getCiphertextLength(plaintext.textLen) + crypto_secretbox_MACBYTES;
      ciphertext.text = (unsigned char*) malloc(ciphertext.textLen);
      /* Now encrypt the plaintext */
      if(crypto_secretbox_easy(ciphertext.text, plaintext.text, plaintext.textLen, plaintext.nonce,
                 key) == 0) {
        //ciphertext.textLen = getCiphertextLength(plaintext.textLen) + CRYPTO_NONCE_BYTES;
        /* Finally, if encryption was successful, send ciphertext back in a packet */
        packetizerSend(&ciphertext);
      }
      /* Free all buffers that were allocated in the meantime */
      free(plaintext.text);
      plaintext.text = NULL;
      free(ciphertext.text);
      ciphertext.text = NULL;
    }
  }
}
