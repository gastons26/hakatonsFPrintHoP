#include <Adafruit_Fingerprint.h>

const String TAG = "fp_crud:";

const String CREATE = "C";
const String READ   = "R";
const String UPDATE = "U";
const String DELETE = "D";
const String DELETEALL = "DA";

const String ERRMSG = "E:";
const String FFOUND = "OK:";
const String INFO = "I:";


SoftwareSerial softSerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&softSerial);
uint8_t id = -1;

String currentCommand = READ;


String inputString = "";
bool stringComplete = false;


void setup() {
  const String MTAG = TAG + "setup:";
  Serial.begin(9600);
  while (!Serial);
  delay(100);
 
  finger.begin(57600);

  if (finger.verifyPassword()) {
//    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println(ERRMSG+"Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
}

void loop() {
  const String MTAG = TAG + "loop:";
  

  if (stringComplete && currentCommand == CREATE) {
    id = readnumber();
    if (id == 0) {
      resetState();
      return;
    }
    while (!  getFingerprintEnroll() );
  
    resetState();
  } else if (stringComplete && currentCommand == UPDATE) {
    resetState();
  } else if (stringComplete && currentCommand == DELETE) {
    id = readnumber();
    if (id == 0) {
       resetState();
       return;
    }
    deleteFingerprint(id);
    
    resetState();
  } else if (stringComplete && currentCommand == DELETEALL) {
    finger.emptyDatabase();
    resetState();
  } else {
    getFingerprintIDez();
    resetState();
  }

  delay(50);
}




uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print(INFO+"Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println(INFO+"Image taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(ERRMSG+"Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(ERRMSG+"Imaging error");
      break;
    default:
//      Serial.println(ERRMSG+"Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(INFO+"Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(ERRMSG+"Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(ERRMSG+"Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(ERRMSG+"Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(ERRMSG+"Could not find fingerprint features");
      return p;
    default:
//      Serial.println(ERRMSG+"Unknown error");
      return p;
  }
  
  Serial.println(INFO+"Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
//  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println(INFO+"Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println(INFO+"Image taken");
      break;
    case FINGERPRINT_NOFINGER:
//      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(ERRMSG+"Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(ERRMSG+"Imaging error");
      break;
    default:
//      Serial.println(ERRMSG+"Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(INFO+"Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(ERRMSG+"Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(ERRMSG+"Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(ERRMSG+"Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(ERRMSG+"Could not find fingerprint features");
      return p;
    default:
//      Serial.println(ERRMSG+"Unknown error");
      return p;
  }
  
  // OK converted!
//  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println(INFO+"Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(ERRMSG+"Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println(ERRMSG+"Fingerprints did not match");
    return p;
  } else {
//    Serial.println(ERRMSG+"Unknown error");
    return p;
  }   
  
//  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println(INFO+"Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(ERRMSG+"Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println(ERRMSG+"Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println(ERRMSG+"Error writing to flash");
    return p;
  } else {
//    Serial.println(ERRMSG+"Unknown error");
    return p;
  }   
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(INFO+"Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(ERRMSG+"No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(ERRMSG+"Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(ERRMSG+"Imaging error");
      return p;
    default:
//      Serial.println(ERRMSG+"Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(INFO+"Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(INFO+"Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(ERRMSG+"Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(ERRMSG+"Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(ERRMSG+"Could not find fingerprint features");
      return p;
    default:
//      Serial.println(ERRMSG+"Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println(INFO+"Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(ERRMSG+"Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println(ERRMSG+"Did not find a match");
    return p;
  } else {
//    Serial.println(ERRMSG+"Unknown error");
    return p;
  }   
  
  Serial.print(finger.fingerID); Serial.println("");

  return finger.fingerID;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
//  Serial.print("finger.fingerID"); Serial.print(finger.fingerID);
  Serial.println(FFOUND + finger.fingerID);
  
  return finger.fingerID; 
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println(INFO+"Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(ERRMSG+"Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println(ERRMSG+"Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println(ERRMSG+"Error writing to flash");
    return p;
  } else {
//    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}




void resetState() {
  const String MTAG = TAG + "resetState:";
  currentCommand = READ;
  inputString = "";
  stringComplete = false;
}


uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


void serialEvent() {
  const String MTAG = TAG + "serialEvent:";
  while (Serial.available() && currentCommand == READ) {
    char inChar = (char)Serial.read();
    
    if (inChar == '\n') {
      stringComplete = true;
      if (inputString == CREATE || inputString == UPDATE || inputString == DELETE  || inputString == DELETEALL) {
        currentCommand = inputString;
      }
    } else {
      inputString += inChar;
    }
  }
}
