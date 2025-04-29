# Secure boot

Within a secure boot not signed software is prevented to run.
Cominit provide secure boot capabilities by:

* checking the signature of the rootfs metadata.
* checking whether unsealing of TPM fails (See documentation here [measured boot](doc/measured_boot/measured-boot.md).)

If one of this applies Cominit should stop the boot process.
In this state it should not enter i.e. a rescue shell in which critical system information can be retrieved.

If unsealing fails, the current PCR values don’t satisfy the policy, so the platform state isn’t trusted. By binding the policy to the appropriate PCRs,  situations as an untrusted kernel or a corrupted initramfs can be detected—because any change extends those PCRs with different hashes.

By doing this checks, Cominit prevents

*  proceeding boot if started from an unknown initramfs
*  running into an unknown kernel


