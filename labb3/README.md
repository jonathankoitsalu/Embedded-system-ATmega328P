# labb3
 
deluppgift 1:

Hela labb3 görs med en naken tritech sköld. (dvs att jag inte kopplar om något).

-knapp: key1 kopplad till digital pin 8(PB0). Digital pin 8 har en interrupt(PCINT0) som aktiveras för deluppgift 2 och 4.
-potentiometer: potentiometer kopplad till analog pin 0.
-led: blåa delen av tritechsköldens RGBled som är kopplad till digital pin 9(PB1) denna pin har PWM via timer1(OC1A). 

Det noteras att ledkretsschemat har en 10 k resistans parallelt med led:en. Vi tar inte med den i vår krets.

Deluppgift 2:

Debounce är byggd på knapp och timer interrupts.
buttonReader läser av knappen nu och sätter timer2 till 0.
Debounce läser knappen på nytt när timern kör en interrupt dvs 10 ms efter att ha läst av en knapphändelse.
Om knappen fortfarande är intryckt eller släppt så bekräftas inputen och printas i executeCommand.

deluppgift 3: potentiometerstyrd ljusstyrka

Timer1 kör pwm på digital pin 9 som styr blått ljus på rgb led:en.
Timer2 startar en AD konversion var 10:e ms
ADC interrupten körs vid slutet av en konversion
Uppmätta adcValue skickas in i pwmVärdet 0CR1A vid nästa timer2 interrupt

deluppgift 4: ANVÄNDARSTYRT BETEENDE

timer1 konfigureras till pwm för att kontrollera blått ljus (digital pin 9) på arduinoskölden
timer2 konfigureras till CTC med 100 hz alltså 10ms period. Den är interruptstyrd.
digital pin 8 interuppt aktiveras för att få tillgång till sköldens key1 knapp.

stateExecuter körs varje gång timer2 tickar eftersom alla states (förutom ledoff) är timerberoende.
stateHandler byter state vid knappinterrupt och skriver ut nuvarande state på serial.