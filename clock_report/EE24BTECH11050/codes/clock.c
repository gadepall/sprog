// 7447 BCD input pins
const int A = 2, B = 3, C = 4, D = 5;
// Display common pins
const int digits[] = {11, 10, 9, 8, 7, 6};

int hours = 17, minutes = 7, seconds = 5;
unsigned long prevMillis = 0;
const int interval = 1000;  // Update time every 1 second

void setup() {
    Serial.begin(9600);

    // Set up 7447 BCD pins as output
    for (int i = A; i <= D; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }

    // Set up display common pins as output
    for (int i = 0; i < 6; i++) {
        pinMode(digits[i], OUTPUT);
        digitalWrite(digits[i], HIGH);  // Start with all displays OFF
    }

    Serial.println("Clock Started!");
}

void loop() {
    unsigned long currentMillis = millis();

    // Update time every second
    if (currentMillis - prevMillis >= interval) {
        prevMillis = currentMillis;
        updateClock();
        Serial.print("Time: ");
        Serial.print(hours); Serial.print(":");
        Serial.print(minutes); Serial.print(":");
        Serial.println(seconds);
    }

    // Refresh the display continuously
    multiplexDisplay();
}

void updateClock() {
    seconds++;
    if (seconds == 60) {
        seconds = 0;
        minutes++;
        if (minutes == 60) {
            minutes = 0;
            hours++;
            if (hours > 24) hours = 0;
        }
    }
}

void multiplexDisplay() {
    for (int i = 0; i < 6; i++) {
        int digitValue = getDigit(i);
        showDigit(digitValue, i);
    }
}

void showDigit(int num, int digitIndex) {
    turnOffDisplays();  
    digitalWrite(digits[digitIndex], HIGH);  // Activate display
    set7447(num);  
    delay(5);  // Small delay to allow visibility
    digitalWrite(digits[digitIndex], LOW); // Turn off display
}

void set7447(int num) {
    digitalWrite(A, num & 1);
    digitalWrite(B, (num >> 1) & 1);
    digitalWrite(C, (num >> 2) & 1);
    digitalWrite(D, (num >> 3) & 1);
}

void turnOffDisplays() {
    for (int i = 0; i < 6; i++) {
        digitalWrite(digits[i], LOW);  // Turn off all displays
    }
}



int getDigit(int index) {
    switch (index) {
        case 0: return hours / 10;    
        case 1: return hours % 10;    
        case 2: return minutes / 10;  
        case 3: return minutes % 10;  
        case 4: return seconds / 10;  
        case 5: return seconds % 10;  
        default: return 0;
    }
}