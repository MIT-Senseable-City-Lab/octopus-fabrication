# **Octopus**
**(Insert main image here.)**



### **Documentation**

This document serves as a guide for fabricating the Octopus. The design consists of three distinct components, each serving specific functions. This documentation provides an overview of the parts, their intended purposes, and recommended printer settings for the Bambu Lab 3D printer.   

---

## **Files Overview**

### `octopus-head.stl`
- **Purpose**:  
  - Houses the **PCB** (and temperature sensor) and an **integrated cooling fan**.  
  - Acts as the top cover for the device, providing access to essential electronic components.

### `octopus-opc.stl` (Optional)
- **Purpose**:  
  - Designed to house the **Particulate Matter sensor (Sensirion SPS30)**.  
  - Enhances functionality by adding air quality sensing capabilities.

### `octopus-bottom.stl`
- **Purpose**:  
  - Encases the **battery** and **GPS module**.  
  - Provides the possibility to attach the device to various surfaces.


---


## **Printer Settings for Bambu Lab**

For optimal fabrication, use the following settings on the **Bambu Lab 3D printer**:

### **General Settings**
- **Layer Height**: 0.2 mm (recommended for standard quality)  
- **Wall Thickness**: 1.2 mm  
- **Infill Density**: 10%-20% (adjust based on strength requirements)  
- **Print Speed**: 50-70 mm/s  
- **Support Type**: Tree (auto), with overhang angle set to 30°  

### **Material Recommendations**
Printing in **white or light colors** is recommended to **minimize heat absorption** and prevent overheating of internal components, especially for outdoor or high-temperature environments.

#### `octopus-head.stl`:
- Material: White PLA or PETG  
- Nozzle Temperature: 230-250°C  
- Bed Temperature: 85-100°C  
- Cooling Fan: Enabled  

#### `octopus-opc.stl`:
- Material: White PLA or PETG  
- Nozzle Temperature: 200-230°C  
- Bed Temperature: 60-80°C  
- Cooling Fan: Enabled  

#### `octopus-bottom.stl`:
- Material: White PLA or PETG  
- Nozzle Temperature: 200-250°C  
- Bed Temperature: 60-100°C  
- Cooling Fan: Enabled  

### **Advanced Settings**
- **Seam Position**: Aligned (for aesthetic purposes)  
- **First Layer Speed**: 20 mm/s for better adhesion.  
- **Adhesion Type**: Brim or Raft (recommended for `octopus-bottom.stl`).  

**N.B.**
- It is generally recommended to place the components in a way that minimizes supports to minimize printing time and maintain the structural integrity of the parts.  
The suggested placement of components  on the print bed is as follows:


![Octopus Diagram](https://i.ibb.co/3mNN2Mk/Screenshot-2024-11-19-130115.png)




---

## **Post-Processing**
- Remove supports carefully to avoid damaging the mounting points.  
- Test-fit each component to ensure proper alignment and functionality before final assembly.  


---


## **Assembly**

### **Overview**
- **No Additional Hardware Needed**:  
  The device uses **integrated screws** directly built into the 3D-printed components, eliminating the need for pins, screws, or external fasteners.  
- **Assembly Time**:  
  Designed for quick assembly, it should take no more than **10-15 minutes**.  

### **Materials Needed**
#### **Electronics**:
- 40x40mm **5V DC Fan**  
- (Optional) **Sensirion SPS30**  

#### **Miscellaneous**:
- **Rubber Bands**


---


## **Step-by-Step Assembly**

**(Add assembly process images here.)**


### **1. `octopus-head.stl`**
- Place the **fan** on the inside of the head in the designated mount.  
- Position the **PCB** in the center of the head.  
- Connect the **fan** to the **5V port** on the back of the PCB.  
- Secure the PCB in place using **rubber bands**, threading them around the surrounding elevated blocks.

### **2. `octopus-opc.stl` (Optional)**
- Position the **Sensirion SPS30** within the middle section.  
- Ensure the back of the Sensirion aligns with the input/output openings in the print.  
- Connect the Sensirion to the **air pollution port** on the PCB (refer to the silkscreen for guidance).  

### **3. `octopus-bottom.stl`**
- Insert the **battery** and then the **GPS module** into their compartments.  
- Pass the battery wire through the assigned hole before connecting it.  
- Connect the **battery** and **GPS** to their respective ports (refer to the silkscreen for port labels).  


---

## **Contact**
For questions or troubleshooting, please contact **[SCL? Simone? ]**.
