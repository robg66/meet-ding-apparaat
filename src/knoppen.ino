int knoppen_selecteer_pin;
int knoppen_potmeter_pin;

long knoppen_laatst_gedrukt = 0;
long knoppen_laatste_loop = 0;

int knoppen_potmeter_laatste_waarde = -1;

void (*knoppen_select_func)(void);
void (*knoppen_pot_func)(int);

void knoppen_init(int selecteer, int pot)
{
  knoppen_selecteer_pin = selecteer;
  knoppen_potmeter_pin = pot;

  attachInterrupt(0, knop_selecteer, FALLING);
}

void knoppen_set_selecteer_callback(void(*f)(void))
{
  knoppen_select_func = f;
}

void knoppen_set_pot_callback(void(*f)(int))
{
  knoppen_pot_func = f;
}

void knop_selecteer()
{
  if (millis() - knoppen_laatst_gedrukt < 250){
    return;
  }
  knoppen_laatst_gedrukt = millis();
  
  log_println("Knop ingedrukt");
  knoppen_select_func();
}

void knoppen_loop()
{
  if (millis() - knoppen_laatste_loop > 10){
    knoppen_laatste_loop = millis();

    int huidige_waarde = knoppen_potmeter_waarde();
    if (knoppen_potmeter_laatste_waarde != huidige_waarde){
      knoppen_potmeter_laatste_waarde = huidige_waarde;
      
      char buffer[50];
      sprintf(buffer, "Potmeter waarde is: %d", huidige_waarde);
      log_println(buffer);
      knoppen_pot_func(huidige_waarde);
    }
  }
}

int knoppen_potmeter_waarde()
{
  // 0 - 1023 -> 0-100
  int knoppen__potmeter_waarde = analogRead(knoppen_potmeter_pin);

  // 0-1000
  knoppen__potmeter_waarde -= 12;
  if (knoppen__potmeter_waarde > 1000) {
    knoppen__potmeter_waarde = 1000;
  } else if (knoppen__potmeter_waarde < 0) {
    knoppen__potmeter_waarde = 0;
  }

  knoppen__potmeter_waarde /= 10;

  return knoppen__potmeter_waarde;
}
