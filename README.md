# PhoneSpecsAPI

phoneSpecsAPI is a **high‑performance REST API** that provides **detailed technical specifications** for popular smartphone models. The API is developed in **C++** using the **Crow (crow.h)** framework and the **nlohmann/json** library.

> Thanks to its performance‑oriented architecture, the API delivers extremely fast responses with very low latency.

---

## Supported Brands

<p align="left">
  <img src="https://cdn.simpleicons.org/samsung/000000" alt="Samsung" width="40" />
  <img src="https://cdn.simpleicons.org/apple/000000" alt="Apple" width="40" />
  <img src="https://cdn.simpleicons.org/xiaomi/000000" alt="Xiaomi" width="40" />
  <img src="https://cdn.simpleicons.org/google/000000" alt="Google" width="40" />
  <img src="https://cdn.simpleicons.org/huawei/000000" alt="Huawei" width="40" />
  <img src="https://cdn.simpleicons.org/nokia/000000" alt="Nokia" width="40" />
</p>

Supported manufacturers:

* Samsung
* Apple
* Xiaomi
* Google
* Huawei
* Nokia

---

## Available Phone Specifications

For each phone model, the API returns comprehensive information including:

* Brand and model name
* Release year
* Processor (CPU)
* Graphics unit (GPU)
* RAM and storage options
* Display size, resolution, and panel type
* Camera specifications (front / rear)
* Battery capacity and charging technologies
* Operating system
* Connectivity features (5G, Wi‑Fi, Bluetooth, etc.)

---

## Technology Stack

* **C++**
* **Crow (crow.h)** – Lightweight and fast HTTP framework
* **nlohmann/json** – JSON parsing and serialization

This technology stack enables the API to remain **extremely fast, efficient, and stable**, even under high request loads.

---

## Example Endpoint Structure

```http
GET phonespecsapi/samsung
GET phonespecsapi/samsung/{model}
GET phonespecsapi/samsung/{model}/{year}
```

### Sample JSON Response

```json
{
  "phones": [
    {
      "brand": "Samsung",
      "model": "Galaxy S23 Ultra",
      "release_year": 2023,
      "price": 1400,
      "hardware": {
        "ram": "12GB",
        "storage": ["256GB", "512GB", "1TB"],
        "cpu": "Snapdragon 8 Gen 2",
        "gpu": "Adreno 740",
        "screen": {"size": "6.8 inch", "resolution": "3088x1440", "refresh_rate": "120Hz"},
        "battery": {"capacity": "5000mAh", "fast_charging": "45W", "wireless_charging": true}
      },
      "camera": {
        "rear": {"main": "200MP", "ultrawide": "12MP", "telephoto": "10MP", "periscope": "10MP", "features": ["OIS", "HDR", "8K video", "100x zoom"]},
        "front": {"main": "12MP", "features": ["HDR", "4K video"]}
      },
      "colors": ["Phantom Black", "Green", "Cream", "Lavender"],
      "features": ["5G", "NFC", "Wireless Charging", "Water Resistant", "Face Recognition", "Fingerprint Sensor", "Samsung Pay", "S Pen"],
      "software": {"OS": "Android 14", "UI": "One UI 6", "SecurityPatch": "2025-01", "preinstalled_apps": ["Samsung Notes", "Galaxy Store", "Samsung Health"]},
      "physical": {"weight": "234g", "dimensions": "163.4x78.1x8.9mm", "material": "Aluminum + Glass", "SIM": "Dual"},
      "sensors": ["Accelerometer", "Gyroscope", "Proximity", "Compass", "Barometer", "Fingerprint", "Ambient light", "Hall", "Step counter", "NFC"],
      "connectivity": ["Wi-Fi 6E", "Bluetooth 5.3", "GPS", "5G", "USB-C 3.2"],
      "media": {"speakers": "Stereo", "audio": ["MP3", "AAC", "FLAC", "WAV"], "video": ["8K@24fps", "4K@60fps"]},
      "security": ["Face Recognition", "Fingerprint", "Knox", "Secure Folder"]
    },
  ]
}
```

---

## Performance

phoneSpecsAPI is designed with performance as a top priority. Thanks to its C++ core and minimal dependencies, it offers:

* Very low response latency
* High requests‑per‑second capacity
* Minimal CPU and memory usage

This makes the API suitable for both personal projects and production‑level systems.

---

## Installation (Summary)

1. Compile using C++17 or later
2. Start the server

---

## License

This project is distributed under a license defined by the author.

---

**PhoneSpecsAPI** – Fast, lightweight, and comprehensive smartphone specifications API.
