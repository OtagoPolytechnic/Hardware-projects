class Plant{
  private:  
    int Hydration_Pin;
    int Nitrogen_Pin;
    int Air_Temp_Pin;
    int UV_Pin;
  
  public:
    float Hydration_Level;
    float Nitrogen_Level;
    float Air_Temp;
    float UV;
    String Plant_Code_Name;
    
    // Constructor to initialize pins and plant name
    Plant(int hydPin, int nitPin, int tempPin, int uvPin, String codeName) {
      Hydration_Pin = hydPin;
      Nitrogen_Pin = nitPin;
      Air_Temp_Pin = tempPin;
      UV_Pin = uvPin;
      Plant_Code_Name = codeName;
      
      // Initialize readings to 0
      Hydration_Level = 0.0;
      Nitrogen_Level = 0.0;
      Air_Temp = 0.0;
      UV = 0.0;
    }
    
    // Read hydration level (0-100%)
    float HydrationLevelRead() {
      int rawValue = analogRead(Hydration_Pin);
      // Convert analog reading (0-1023) to percentage (0-100%)
      // Assuming higher analog value = more moisture
      Hydration_Level = map(rawValue, 0, 1023, 0, 100);
      return Hydration_Level;
    }
    
    // Read nitrogen level (0-100 ppm estimated)
    float NitrogenLevelRead() {
      int rawValue = analogRead(Nitrogen_Pin);
      // Convert to estimated nitrogen level in ppm
      Nitrogen_Level = map(rawValue, 0, 1023, 0, 100);
      return Nitrogen_Level;
    }
    
    // Read air temperature (in Celsius)
    float AirTempRead() {
      int rawValue = analogRead(Air_Temp_Pin);
      // Convert to temperature (assuming TMP36 sensor: 10mV/°C, 500mV offset)
      float voltage = (rawValue * 5.0) / 1023.0;
      Air_Temp = (voltage - 0.5) * 100;
      return Air_Temp;
    }
    
    // Read UV index (0-11 scale)
    float UVRead() {
      int rawValue = analogRead(UV_Pin);
      // Convert to UV index scale (0-11)
      UV = map(rawValue, 0, 1023, 0, 11);
      return UV;
    }
    
    // Method to print all sensor readings
    void printReadings() {
      Serial.println("=== " + Plant_Code_Name + " Sensor Readings ===");
      Serial.println("Hydration Level: " + String(Hydration_Level) + "%");
      Serial.println("Nitrogen Level: " + String(Nitrogen_Level) + " ppm");
      Serial.println("Air Temperature: " + String(Air_Temp) + "°C");
      Serial.println("UV Index: " + String(UV));
      Serial.println();
    }
    
    // Method to read all sensors at once
    void readAllSensors() {
      HydrationLevelRead();
      NitrogenLevelRead();
      AirTempRead();
      UVRead();
    }
};

// Create testPlant object
Plant testPlant(A0, A1, A2, A3, "TestPlant_001");

void setup() {
  Serial.begin(9600);
  Serial.println("Plant Monitoring System Starting...");
  
  delay(2000);
}

void loop() {
  // Read individual sensors using testPlant object methods
  Serial.println("Reading individual sensors:");
  
  float hydration = testPlant.HydrationLevelRead();
  Serial.println("Hydration reading: " + String(hydration) + "%");
  
  float nitrogen = testPlant.NitrogenLevelRead();
  Serial.println("Nitrogen reading: " + String(nitrogen) + " ppm");
  
  float temperature = testPlant.AirTempRead();
  Serial.println("Temperature reading: " + String(temperature) + "°C");
  
  float uv = testPlant.UVRead();
  Serial.println("UV reading: " + String(uv));
  
  Serial.println();
  
  // Alternative: Read all sensors at once and print
  Serial.println("Reading all sensors at once:");
  testPlant.readAllSensors();
  testPlant.printReadings();
  
  // Wait 5 seconds before next reading
  delay(5000);
}