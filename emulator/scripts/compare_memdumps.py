with open('good_memdump.bin', 'rb') as good_memdump:
    with open('bad_memdump.bin', 'rb') as bad_memdump:
        for address in range(0x10000):
            good_value = good_memdump.read(1)
            bad_value = bad_memdump.read(1)
            if good_value != bad_value:
                print(f'{hex(address)}: {good_value.hex()} != {bad_value.hex()}')