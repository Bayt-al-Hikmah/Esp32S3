## Objectives
- Introduction to Electricity
- Understand Electric Circuits From Basic Connections to Microcontrollers
## Introduction to Electricity
### Introduction
We live in the electric century. Everywhere we look around us, we see electrical and digital devices. Our homes run on electrical power. Our phones, TVs, and even our cars are becoming electric. But what is electricity, and where does it come from?
### Atoms
To understand electricity, we must start with the smallest building blocks of matter: **atoms**. Everything around us is made of atoms. Each atom consists of three main particles: **protons**, **neutrons**, and **electrons**.

- **Protons** carry a positive charge
- **Electrons** carry a negative charge
- **Neutrons** have no charge

Protons and neutrons are packed together in the center of the atom, called the **nucleus**, while electrons move around the nucleus in a cloud-like region.


<img src="./attachments/atom.png" height="250px">

In a normal state, an atom has equal numbers of protons and electrons, so it is electrically neutral. However, atoms can **gain or lose electrons**.

- If an atom **loses electrons**, it becomes **positively charged**
- If it **gains electrons**, it becomes **negatively charged**

Some materials hold their electrons tightly, while others allow electrons to move more easily. This difference is key to understanding electricity.
### Electricity
Electricity is a phenomenon that occurs when electrons move from negatively charged materials to positively charged ones. In nature we have two type of electricity, static and dynamic.   
#### Static Electiricty
The earliest form of electricity humans experienced is called static electricity. One of the most powerful natural examples of static electricity is lightning. During a storm, electric charges separate inside a cloud. The top of the cloud becomes positively charged, while the bottom becomes negatively charged. The negative charge at the bottom pushes electrons away from the ground, making the ground positively charged. This creates a strong electric field between the cloud and the Earth. When the electric field becomes strong enough, the air can no longer block the charges. Electricity suddenly flows between the cloud and the ground. This fast movement of electrons produces the bright flash we see as lightning.     

<img src="./attachments/lightning.png"/>

Another early encounter with static electricity happened when people rubbed two materials together. This process creates a charged surface: one object becomes negatively charged by gaining electrons, while the other becomes positively charged by losing electrons. When we bring two objects with the same type of charge close to each other, they repel one another. On the other hand, when we bring materials with opposite charges together, they attract each other. The force of attraction and repulsion between charges can be described by a physical law and expressed using a mathematical equation.
$$F = \frac{k \times (q_1 \times q_2)}{ r^2}$$
- **F** = electric force between the two charges (in newtons)
- **k** = Coulomb’s constant (about 9 × 10⁹ N·m²/C²)
- **q₁ and q₂** = the two electric charges (in coulombs)
- **r** = the distance between the charges (in meters)


<img src="./attachments/static_electricity.png">


#### Dynamic Electricity
Unlike static electricity which is based on charges at rest, dynamic electricity, consists of the continuous flow of electrons. This flow is called electric current. It is more useful because we can control it more easily, since it is continuous and does not occur only for a brief moment. There are many ways to generate this type of continuous current; one of them is through chemical reactions.

To generate electricity from chemical reactions, one of the most common methods is using **acids**. Acids are special substances that dissolve in water and produce positive and negative ions. For example, when hydrochloric acid (HCl) dissolves in water, it reacts with water molecules and forms hydronium ions (H₃O⁺) and chloride ions (Cl⁻). The hydronium ions are positively charged and make the solution reactive.

<img src="./attachments/acide.png" />

Zinc (Zn) is a metal that can easily lose electrons and become positively charged. When we place zinc in an acidic solution, zinc atoms give away two electrons and turn into zinc ions:

$$\text{Zn} \rightarrow \text{Zn}^{2+} + 2e^-$$


<img src="./attachments/zinc.png" />

These zinc ions then combine with chloride ions in the solution to form zinc chloride. 
$$Zn^{2+}+2Cl^-\rightarrow ZnCl_2$$
Because zinc releases electrons, the zinc electrode becomes negatively charged. Since electrons have the same negative charge, they repel each other. This repulsion creates a force that pushes electrons away from the zinc.

Now, if we place another material, such as copper (Cu), which does not easily lose or gain electrons, into the solution and connect it to the zinc with a wire, we create a path for the electrons to move. The electrons travel through the wire from the negatively charged zinc to the copper.
At the copper side, these electrons are transferred to the hydronium ions in the solution. Each two electrons react with hydronium ions to produce water and hydrogen gas:

$$2H_3O^+ + 2e^- \rightarrow 2H_2O + H_2$$​

This continuous movement of electrons through the wire is what we call electric current. By controlling this chemical reaction and the path of the electrons, we can generate a steady flow of electricity, which is the basic principle behind many batteries.

<img src="./attachments/batery.png">

### Electricity and Magnetism
Electricity and magnetism were not always known to be connected. Electricity was about moving charges, while magnetism was only associated with special materials like iron and natural magnets. This changed in the 19th century when scientists began to notice surprising interactions between electric currents and magnetic forces.

One of the key discoveries came when scientists such as Pierre-Simon Laplace and others studied how electric currents affect each other. They observed that when an electric current flows through a wire, it creates a force that can push or pull another current nearby. By changing the current, the force also changed. This showed that electricity was somehow creating a magnetic effect around the wire.

<img src="./attachments/laplace_expirement.png" />

Soon after, experiments by André-Marie Ampère confirmed that every moving electric charge produces a magnetic field. When current flows, a circular magnetic field forms around the wire. The stronger the current, the stronger the magnetic field. 

<img src="./attachments/magnitism.png" />

Scientists then tried to strengthen this magnetic effect. They discovered that instead of using a straight wire, they could wind the wire into loops called **coils**. When current flows through a coil, the magnetic fields from each loop add together. This creates a much stronger and more concentrated magnetic field, similar to a bar magnet. If they placed an iron core inside the coil, the magnetic field became even stronger. These devices are called **electromagnets**, and their strength can be controlled simply by changing the current.

<img src="./attachments/coil.png" />

This principle led to one of the most important inventions in technology: the electric motor. In a motor, coils carrying current are placed inside magnetic fields. The interaction between the magnetic field of the coil and the external magnetic field produces forces that cause rotation. By switching the current at the right time, the motor continues to spin. This allows electrical energy to be converted into mechanical motion, which is used in fans, pumps, electric cars, and many machines.

<img src="./attachments/motor.png"/>

After this discovery, scientists asked an important question. If electric current creates magnetic fields, could magnetic fields also create electricity? The answer came from the work of Michael Faraday. In his famous experiments, he moved a magnet near a coil of wire. He observed that when the magnetic field through the coil changed, an electric current appeared in the wire. However, if the magnet did not move, no current was produced. This showed that changing magnetic fields generate electricity.

<img src="./attachments/generator.png" />

The principle behind this effect is called **electromagnetic induction**. When a magnetic field changes around a conductor, it pushes electrons and makes them move. This movement of electrons creates an electric current. The faster the magnetic field changes, the stronger the current. This is the basic principle used in generators. In power plants, turbines rotate magnets or coils, continuously changing the magnetic field and producing electricity.
###  Conductors and Insulators
We now know how to generate electricity and how electric current can create magnetic effects. However, another important question soon appeared: how do different materials react when electricity passes through them?     
One of the early scientists who studied how electricity travels through materials was Georg Simon Ohm. Through many experiments, he found that the flow of electric current depends on both the material and the applied voltage. He noticed that in some materials, electric charges could move freely, producing a strong current. In other materials, the current was very weak or almost zero. This showed that the internal structure of materials plays a key role in electrical conduction.    

In conductors, such as copper, aluminum, and silver, outer electrons are free to move throughout the material, forming what is sometimes called an “electron sea.. These electrons can move easily when an electric field is applied. When we connect a conductor to a battery, the electric field pushes these free electrons, creating a continuous flow of charge. This is why metals are commonly used to make wires and electrical circuits. The easier the electrons can move, the better the material conducts electricity.      

<img src="./attachments/conductor.png" />

In contrast, insulators such as rubber, plastic, glass, and dry wood behave very differently. In these materials, electrons are tightly bound to their atoms and cannot move freely. When an electric field is applied, the electrons only shift slightly but do not travel through the material. As a result, almost no current flows. This is why insulators are used to protect us from electric shocks and to cover wires, preventing electricity from escaping.

<img src="./attachments/insulator.png" />

### Current, Potential, and Resistance
To study electricity and electrical phenomena, we focus on three main concepts: electric current, electric potential, and resistance. These ideas help us understand how electricity moves, why it moves, and what can slow it down or control it.
#### Electric Current
Electric **current** represents the flow of electrons from one place to another. When electrons move through a wire or a material, we say that an electric current is present. The concept of electric current was studied in detail by scientists such as André-Marie Ampère, who showed that current is related to moving electric charges. Electric current is defined as the amount of electric charge that passes through a section of a conductor in a certain amount of time.  
In metals, current is caused by the motion of free electrons. When a battery or power source is connected, an electric field appears inside the conductor. This field pushes the electrons, causing them to drift in one direction. Although the electrons move slowly, the effect of the electric field spreads very quickly through the circuit.  
We measure electric current in amperes (A). A larger current means more charges are moving each second. For example, a small current is enough for electronic devices, while large currents are needed for motors or heaters.
#### Electric Potential (Voltage)
Electric **potential** often called **voltage**, represents the difference in electric charge or energy between two points. This difference is what pushes electrons to move. It was explored by scientists such as Alessandro Volta. It represents the electric energy available to move charges between two points. The greater the potential difference, the stronger the push on the electrons.  
An easy way to imagine voltage is to compare it to water pressure. Just as higher pressure pushes water through pipes, a higher electric potential pushes electrons through a circuit. Batteries and generators create this potential difference, allowing current to flow.  
Voltage is measured in volts (V). A higher voltage does not always mean more current, because the current also depends on the resistance of the circuit. 
#### Resistance
Resistance represents how much a material opposes the flow of electric current. Some materials allow electrons to move easily, while others slow them down. Resistance is important because it controls the amount of current and protects electrical systems from damage, it was carefully studied by Georg Simon Ohm, who discovered the relationship between current, voltage, and resistance.   
In a conductor, electrons collide with atoms as they move. These collisions slow down the electrons and convert electrical energy into heat. Materials such as copper have low resistance, which is why they are used in wires. Other materials, such as tungsten, have higher resistance and are used in heating elements or light bulbs.  
Resistance depends on several factors, including the material, the length of the conductor, and its thickness. It is measured in ohms (Ω).   
Together, current, potential, and resistance form the foundation of electrical science. Their relationship is summarized in a simple law known as **Ohm’s law**, which connects these three concepts and allows us to predict how electric circuits behave.
$$V=R \times I$$
### Semiconductors
After understanding conductors and insulators, scientists discovered that some materials do not behave like either of these two groups. These materials are called semiconductors because their ability to conduct electricity is between that of conductors and insulators. Their electrical behavior can change depending on external conditions such as voltage, current, temperature, or light. This makes them extremely useful and unique.   
In semiconductors such as silicon and germanium, electrons are not completely free as in conductors, but they are also not tightly bound as in insulators. Under normal conditions, only a small number of electrons can move, so the current is weak. However, when we apply a voltage, increase the temperature, or shine light on the material, more electrons gain enough energy to move. As a result, the conductivity of the material increases.   
For example, when temperature rises, atoms vibrate more strongly. This extra energy allows some electrons to escape their atoms and become free to move. Similarly, when light hits a semiconductor, the energy of the photons can free electrons. This principle is used in devices such as light sensors and solar cells.

<img src="./attachments/light_sonsor.png"/>

Another important property of semiconductors is that their conductivity can be controlled by adding small amounts of impurities, a process called doping. By introducing specific atoms, scientists can create materials with either extra free electrons or missing electrons (called holes). This allows precise control of electric current inside the material.
#### P-N Junction
There are two main types of doped semiconductors. In **n-type semiconductors**, atoms that have extra electrons are added. These extra electrons can move easily and carry electric current. In **p-type semiconductors**, atoms that have fewer electrons are introduced. This creates holes, which act like positive charge carriers because nearby electrons can move to fill them, making the holes appear to move.

<img src="./attachments/N&P_semiconductor.png"/>

When a p-type semiconductor is joined with an n-type semiconductor, a special structure called a p-n junction is formed. At this junction, free electrons from the n-side naturally move toward the holes in the p-side, filling some of them. This movement creates a depletion region, a barrier where most electrons cannot pass because they lack enough energy.   
If we connect an energy source so that the positive terminal is on the p-side and the negative terminal on the n-side (forward bias), the applied voltage reduces the barrier. Electrons gain enough energy to cross the depletion region and recombine with holes, allowing a continuous electric current to flow.  
However, if we reverse the connection, the barrier increases. Electrons are pulled away from the junction, and very few can cross, so almost no current flows. This the basic idea behind many semiconductor devices. One of the simplest and most important examples is the diode, which allows electric current to flow in only one direction. 

<img src="./attachments/Diode.png"/>

## Electric Circuits: From Basic Connections to Microcontrollers
Now we understand what electricity is, where it comes from, how it is generated, and how different materials react when an electrical current passes through them. Let’s move to the next step: using electricity to power materials and build useful devices and tools.
### Electric Circuit
When we connect a source of electricity to different components, we create what is called an electric circuit. An electric circuit is a closed path that allows electric current to flow from the power source, through the connected materials, and back to the source.  
In order for electricity to flow, the circuit must be complete (closed). If the path is broken or open, the current cannot move, and the device will not work. 

Sometimes we want to add control to our circuits and have the ability to open and close the loop without plugging and unplugging components. To do this, we use a switch. A switch allows us to open and close the circuit, which controls the flow of electricity.   
A basic electric circuit usually has three main parts
1. **Power source** such as a battery or generator, which provides electrical energy.
2. **Conductors (wires)**  which allow the current to travel through the circuit.
3. **Load** the device that uses electrical energy, such as a lamp, motor, or heater.

<img src="./attachments/electric_circuit.png" height="300px"/>

When we connect electrical components in a circuit, we can arrange them in different ways. The two most common types of wiring are series and parallel. Each type has different properties and uses.
#### Series Wiring
In a series circuit, the components are connected one after another in a single path. This means that the electric current flows through each component in the same order.

Because there is only one path for the current, all components share the same current. However, the voltage is divided between them. For example, if we connect several lamps in series, each lamp receives part of the total voltage.

One important characteristic of a series circuit is that if one component stops working or the circuit is broken at any point, the whole circuit stops. This means all devices turn off. 

<img src="./attachments/series_wiring.png" height="250px" />

#### Parallel Wiring
In a parallel circuit, the components are connected across the same two points of the power source. This creates multiple paths for the electric current.   
In this case, each component receives the same voltage, but the current is divided between the different paths. Because of this, each device can work independently.  
If one component stops working, the others continue to operate. For example, in modern houses, if one lamp burns out, the rest of the lights still work.

<img src="./attachments/parallel_wiring.png" height="350px"/>

### Designing Circuits
We have explored how to create basic electric circuits, and we discovered the difference between series and parallel wiring. These ideas do not apply only to loads and components like lamps or motors. We can also use series and parallel wiring when we design the control parts of a circuit, such as switches.

#### Switches in Series
When switches are connected in series, the electric current must pass through all of them. This means every switch must be closed for the circuit to work. If any switch is open, the current stops and the device turns off.  
This type of design is useful when we want several conditions to be met before a device operates. For example, safety systems often use series switches. 

<img src="./attachments/seris_switches.png" />

#### Switches in Parallel
When switches are connected in parallel, the current has more than one path. This means that closing any one of the switches will complete the circuit and turn the device on.   
This design is useful when we want to control the same device from different locations. A common example is the lighting system in large rooms, corridors, or staircases, where a lamp can be turned on or off from more than one place.

<img src="./attachments/parallel_switch.png" />

#### Control and Conditional Circuits
We can do much more with electrical circuits than simply turning devices on and off. By combining different components such as semiconductors, sensors, and control elements, we can create more complex circuits. These circuits can perform logical operations, make decisions, and produce specific output voltages or currents based on certain conditions.

In these systems, sensors are used to detect changes in the environment, such as light, temperature, pressure, or motion. The information from the sensors is then processed by electronic components like transistors, diodes, or integrated circuits. These components, made from semiconductor materials, can control the flow of electricity automatically.

For example, a light sensor can detect darkness and turn on a lamp without human intervention. A temperature sensor can activate a cooling fan when the temperature becomes too high. These are called conditional circuits because the output depends on a specific condition.

Semiconductors play a key role in this process. Devices such as transistors act as electronic switches or amplifiers, allowing circuits to operate quickly and reliably. 

<img src="./attachments/auto_light.png" />

Here example of an automatic night light that turns on a lamp when the surrounding area becomes dark. It uses a light sensor called an LDR (Light Dependent Resistor) to detect the level of light. When there is enough light, the LDR has low resistance, which prevents the transistor from turning on, so the lamp stays off. When it becomes dark, the resistance of the LDR increases, allowing a small current to reach the base of the transistor. The transistor then acts as an electronic switch and allows current to flow from the battery to the lamp. The resistor in the circuit helps control the current and adjusts the sensitivity of the sensor.
#### Integrated Circuits
When we move from simple experiments to real-world products, our circuits must become smaller, faster, and more reliable. Building large systems from many separate components requires more space, more wiring, and increases the risk of failure. To overcome these challenges, engineers developed integrated circuits, which combine many electronic components into a single, compact device.

An integrated circuit (IC) is a tiny chip made from a semiconductor material, usually silicon. Inside this chip, components such as transistors, resistors, and capacitors are fabricated and interconnected. Instead of assembling and wiring each part individually, the entire circuit is manufactured as one compact unit. This approach reduces size, improves performance, lowers cost, and significantly increases reliability.

An integrated circuit provides external pins (or terminals) that allow us to connect power, inputs, and outputs to the device. This makes it possible to use complex and advanced circuits without needing to understand all the internal details of how they operate.

Inputs are signals that come from sensors, switches, or other circuits. The IC processes these signals using its internal electronic components and logic. It then produces outputs that can drive LEDs, motors, displays, or other devices. In this way, an integrated circuit functions like a “black box”: we provide inputs, it processes the information internally, and it delivers the desired outputs.

<img src="./attachments/IC.png" />

### Microcontrollers
As systems became more complex, engineers began to face some limitations with traditional integrated circuits. Although ICs are powerful and reliable, they are often designed to perform a specific task. They operate like a “black box,” meaning we usually cannot access or modify their internal behavior. If we need to change even a small function, improve performance, or add new features, we often must redesign the entire circuit or develop a new chip. This process is expensive, time-consuming, and not practical for many modern applications where flexibility is important.  
To overcome these limitations, engineers developed microcontrollers. A microcontroller is a special type of integrated circuit that contains a small computer inside a single chip. Instead of being fixed for one task, a microcontroller can be programmed to perform different functions depending on the needs of the system. This makes it much more flexible and adaptable than traditional ICs.   
The basic structure of a microcontroller includes three main parts. First, it has a processor (CPU), which is the “brain” of the system and is responsible for executing instructions. Second, it contains memory, where programs and data are stored. This memory usually includes flash memory for the program and RAM for temporary data. Third, it includes input and output (I/O) ports that allow the microcontroller to communicate with sensors, switches, motors, displays, and other electronic components. Many microcontrollers also include built-in peripherals such as timers, communication modules, and analog-to-digital converters, which simplify the design of electronic systems.

<img src="./attachments/micro_controller.png" />

Microcontrollers are programmed using software. Engineers write a program, usually in languages such as C or Python, and upload it to the microcontroller using a computer. This program tells the microcontroller how to read inputs, process information, and control outputs. If the system requirements change, we only need to modify the program and upload a new version, without redesigning the hardware.
