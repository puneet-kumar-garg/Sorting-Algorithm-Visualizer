The Sorting Algorithms Visualizer is a C++ application built using SDL2 and SDL_ttf that graphically demonstrates how different sorting algorithms operate in real time. It helps users understand how sorting works by visually showing comparisons, swaps, and transitions.
The app displays an array of vertical bars whose heights represent numeric values and allows users to control sorting speed, resize the array, and track the number of comparisons and swaps performed.

⸻

⚙️ Features
	•	🎥 Real-time sorting animation
	•	🧠 Algorithms: Bubble, Insertion, Selection, Merge, and Quick Sort
	•	🕹️ Interactive GUI with buttons for algorithm selection, shuffle, resize, and start
	•	⚡ Adjustable speed control
	•	🔢 Displays comparisons and swaps count dynamically
	•	🧱 Resize array dynamically (2–230 elements)
	•	💡 Smooth transitions using SDL2 rendering

🧰 Tech Stack
	•	Language: C++17
	•	Libraries: SDL2, SDL2_ttf
	•	IDE: Visual Studio Code / CLion / Xcode
	•	Platform: macOS / Linux / Windows

1️⃣ Install Dependencies

macOS:
brew install sdl2 sdl2_ttf

2️⃣ Run
make && make run


📊 Displayed Statistics
	•	🧮 Number of comparisons
	•	🔁 Number of swaps
	•	⏱️ Sorting speed (ms per step)

<img width="1136" height="864" alt="image" src="https://github.com/user-attachments/assets/da4c7fec-febe-43e6-b506-da285ae2319a" />

🚀 Future Enhancements
	•	Add Pause / Resume button
	•	Add Heap Sort, Shell Sort, and Radix Sort
	•	Use color highlights for active comparisons
	•	Display time elapsed
	•	Add sound effects for swaps

⸻

💡 Conclusion

This project offers an interactive and visual way to learn sorting algorithms using C++ and SDL2.
It helps users see the logic behind sorting through animated comparisons and real-time metrics, making it an ideal educational and practical demonstration of algorithm efficiency.


🆕 New Update (Latest Commit)

This update improves the visualization experience by adding colored outlines to each bar and changing the bar color when the array is completely sorted. Now, each bar has a clear boundary for better visibility, and once sorting completes, the bars smoothly transition to a green color, indicating successful completion of the sorting process. These enhancements make it easier to distinguish array elements during sorting and provide a visually rewarding finish animation.

