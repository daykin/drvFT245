#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libftdi1/ftdi.h>

#define FT245_VENDOR_ID 0x0403
#define FT245_DEVICE_ID 0x6001

int main(void)
{
    int ret;
    struct ftdi_context *ftdi;
    struct ftdi_version_info version;
    if ((ftdi=ftdi_new())==0){
            fprintf(stderr, "ftdi_new failed :(\n");
    }
    version = ftdi_get_library_version();
    printf("Initialized libftdi %s (major: %d, minor: %d, micro: %d, snapshot ver: %s)\n",
    version.version_str, version.major, version.minor, version.micro,
    version.snapshot_str);

    if ((ret = ftdi_usb_open(ftdi, FT245_VENDOR_ID, FT245_DEVICE_ID)) < 0){
            fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
            ftdi_free(ftdi);
            return EXIT_FAILURE;
    }

    unsigned char pin_mask = 0x07; //D0,D1,D2
    unsigned char bitbang_mode = BITMODE_BITBANG;

    if((ret=ftdi_set_bitmode(ftdi, pin_mask, bitbang_mode)) < 0){
        fprintf(stderr, "failed to set bitmode.\n");
    }

    unsigned char pins=0;
    for(int i=0;i<100;i++){
        usleep(50000);
        pins = rand() % 8; 
        ftdi_write_data(ftdi, &pins, 1);
    }
    pins=0;
    sleep(1);
    ftdi_write_data(ftdi, &pins, 1);
    if ((ret = ftdi_usb_close(ftdi)) < 0){
            fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
            ftdi_free(ftdi);
            return EXIT_FAILURE;
    }
    ftdi_free(ftdi);
 
    return EXIT_SUCCESS;
}