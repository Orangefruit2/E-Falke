#ifndef PI300_CONTROL_MSG
#define PI300_CONTROL_MSG

#include <stdint.h>

/* Datastructure of the PI300 control message */
typedef struct{
	uint8_t adresse;
	uint16_t stellwert; //PWM Sollwert an den Steller in µs (1200 = 0%; 1700= 100%)
	uint16_t akkuspannung; // Spannungsmessung Auflösung in 10mV
	uint16_t akkustrom;		// Akkustrom  512 = 0A; I in A =( I_Roh - 512 )* 1,563  / 2
	uint16_t motordrehzahl;	//Drehzalwert in 1/min
	uint8_t akkutemperatur;	//0-255°C
	uint8_t motortemperatur;	//0-255°C
	uint8_t pi300_temperatur;	//0-255°C
	uint16_t bms_statusword;	// BMS Fehlerinformation (P_Sollwerthoheit=4-5); Blattsollwinkel 0-110  (P_Sollwerthoheit=6)
	int16_t motorstrom;		//Motorstrom in A
	uint16_t istKapazitaetAkku;	// Restkapazität des Akkus (Auflösung 0,1Ah)
	uint16_t betriebsminuten;		// Betriebsminutenzähler seit Auslieferung
	uint8_t steuerbyte;		/* Bit 0 = Reglerfreigabe		Bit 8	   Err_i_charge_max Alias Errorword.8                                           'Fehler Ladestrom über Ladezweig zu hoch               => Ladekanal AUS + Kapazitätszähler Ladezweig AUS
													Bit 1 = Automatik		Bit 9	   Err_i_discharge_max Alias Errorword.9                                        'Fehler Entladestrom über Ladezweig zu hoch            => Ladekanal AUS + Kapazitätszähler Ladezweig AUS
													Bit 2 = Taster Reglerfreigabe		Bit 10	   Err_hibernation Alias Errorword.10                                           'Hibernation = Winterschlaf angewählt                  => ---
													Bit 3 = Reversieren aktiv		Bit 11	   Err_chg_sensor Alias Errorword.11                                            'Fehler Ladestromsensor                                => Prüfung beim Einschalten -> Ladekanal Aus
													Bit 4 = Lebensbit BMS Kommunikation 		Bit 12	   Err_dischg_sensor Alias Errorword.12                                         'Fehler Entladestromsensor                             => Prüfung beim Einschalten -> Entladekanal Aus
													Bit 5 = BMS Abregelung aktiv		Bit 13	   Err_discharge_end_sdc Alias Errorword.13                                     'Fehler Akku tiefentladen bei SDC = 1                   => Meldung an SDC => Power OFF
													Bit 6 = Reserve		Bit 14	   Err_emergency_stop Alias Errorword.14                                        'Fehler Not Aus Powerswitch                            => Powerswitch  AUS
													Bit 7 = Propellerwinkelsteuerung ok =1; Error = 0 */
	uint8_t errorbyte;	/* Bit 0 = Unterspannung Akku; Spannungsgrenzwert unterschritten		 
													Bit 1 =Übertemperatur Endstufe Drehzahlsteller; Grenzwert überschritten	
													Bit 2 = Übertemperatur Motor; Grenzwert am Eisenkern überschritten	
													Bit 3 = Übertemperatur Akku; Grenzwert  überschritten	
													Bit 4 = Stromgrenze Motor; Grenzwert überschritten	
													Bit 5 = Abschaltung nach Grenzwertabregelung	
													Bit 6 = Keine Freigabe von extern	
													Bit 7 = Poti nicht auf Minimalstellung zur Aktivierung */
	uint8_t poti_istwert;	// Poti (Gashebel) Istwert in 0-100%
	uint8_t cr;			// Return (13)
	uint8_t checksum;	// Checksum = byte1+byte2 …+byte24+13(dez)+170(dez) ; checksum as byte
} pi300_control_msg;

/* Returns true or false depending on whether the checksum in the pi300_control_msg msg fits to the rest of the 
 * message. 
 * ! Algorithm for computing the checksum was guessed from the documentation from Geiger, might be incorrect!
*/
static inline uint8_t checksumOK(pi300_control_msg *msg){
	uint8_t* msgPtr = (uint8_t*)msg;
	
	uint8_t checksum = 13+ 170;
	
	for(int i = 0; i<sizeof(msg)-1;i++){
		checksum = checksum + msgPtr[i];
	}
	
	return checksum == msg->checksum;
}

#endif
