#ifndef STEGGIT_H
#define STEGGIT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encode a message into a PNG image
 * @param input Path to input PNG file
 * @param output Path to output PNG file
 * @param message Message to encode
 * @param header Optional header (pass NULL for default "$$")
 * @param error_message Output parameter for error messages (caller must free)
 * @return 0 on success, non-zero on error
 */
int encode_png(const char *input, const char *output, const char *message,
               const char *header, char **error_message);

/**
 * Encode a message into a JPEG image
 * @param input Path to input JPEG file
 * @param output Path to output JPEG file
 * @param message Message to encode
 * @param header Optional header (pass NULL for default "$$")
 * @param error_message Output parameter for error messages (caller must free)
 * @return 0 on success, non-zero on error
 */
int encode_jpeg(const char *input, const char *output, const char *message,
                const char *header, char **error_message);

/**
 * Decode a message from a PNG image
 * @param input Path to input PNG file
 * @param header Optional header (pass NULL for default "$$")
 * @param error_message Output parameter for error messages (caller must free)
 * @return Decoded message (caller must free), or NULL on error
 */
char *decode_png(const char *input, const char *header, char **error_message);

/**
 * Decode a message from a JPEG image
 * @param input Path to input JPEG file
 * @param header Optional header (pass NULL for default "$$")
 * @param error_message Output parameter for error messages (caller must free)
 * @return Decoded message (caller must free), or NULL on error
 */
char *decode_jpeg(const char *input, const char *header, char **error_message);

#ifdef __cplusplus
}
#endif

#endif // STEGGIT_H
