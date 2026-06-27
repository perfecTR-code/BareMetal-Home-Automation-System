# BareMetal-Home-Automation-System

## What I implemented
1. Bit masking
2. Hardware management (Clock Control, Gate Control, AFR, OSPEEDR, MODER...)
### 3. Timer logic
Setting Prescaler and Auto-Load. I have transformed the main clock freq (16MHZ) to 1us to make clean calculations. Auto-Load value is about high limit that timer can reach. When it reaches the Auto-Load value it gets resetted.
### Hardware Safety
I have opened the preload register to prevent my servo motor's glitching.The preload register is kinda the temporary input room.
