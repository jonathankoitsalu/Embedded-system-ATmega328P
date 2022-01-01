# labb4
 
del 1: KONFIGURERA I2C 

För att testa I2C skrivs 'J' till EEPROM_ADDR (0x10) för att sedan läsas ut och printas på screen

del 2: IMPLEMENTERA OCH  DEMONSTRERA EEPROM -KOMMUNIKATION  

eeprom_read_byte() testas med readEepromSerialNumber() som läser av eeprom:ens manufacturer code, device code och 32-bit serial number och skriver ut det på screen.
eeprom_write_byte() testas med writeName() och readName() som skriver in "Jonathan" i eepromen och printar ut det från eepromen till screen.

del 3: UTÖKAD FUNKTIONALITET  

eeprom_write_page() testas med 4 stycken 8-teckens stringar som skrivs in i varsitt "page". Minnesadressen inkrementeras med 8 steg för varje page.
eeprom_sequential_read() testas genom att skriva ut dessa 4 page:s som skrivs in.
eeprom_sequential_write() denna funktion delar upp data i pages (8-teckens strängar) vilka sparas i tur och ordning i eepromet med eeprom_write_page(). Om sista bytes:en inte fyller ut en page så sparas dom en byte i taget med eeprom_write_byte.
eeprom_sequential_write() är i main.c.