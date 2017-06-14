#ifndef PI300_POLLING_MSG
#define PI300_POLLING_MSG

#include <stdint.h>

/*  Datastructure of the PI300 polling message */
typedef struct
{
	uint8_t adresse;	// Adressbyte (121dez. für PI300-Sollwertübertragung)
	uint8_t reserve[6];
	uint16_t soll_blattwinkel;	// Sollwert Blatteinstellwinkel für Betrieb mit Elektronischer Verstellnabe [Wertebereich 0-110] Auflösung 0,1°
	uint16_t soll_stellgrad;		// Sollwert für den PI300 (200-700 = min. bis max) 200 = 0%; 700 = 100%
	uint8_t CR;		// Return(13)
	uint8_t checksum;	// Checksum = byte1+byte2 …+byte12+170(dez) ; checksum as byte (? Nur lowbyte verwenden!)
}pi300_polling_msg;


/* Returns a polling message as defined in the documentation from Geiger */
static inline pi300_polling_msg getExampleMsg(){

	pi300_polling_msg polling_msg_example;
	
	polling_msg_example.adresse = 121;
	for(int i = 0;i<6;i++){
		polling_msg_example.reserve[i] = 0;
	}
	polling_msg_example.soll_blattwinkel = 60;
	polling_msg_example.soll_stellgrad = 1200;
	polling_msg_example.CR = 13;
	polling_msg_example.checksum = 1564;
	
	return polling_msg_example;
}

#endif
