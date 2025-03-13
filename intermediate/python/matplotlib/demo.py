"""
    SPDX-FileCopyrightText: 2025 Julian Amann <dev@vertexwahn.de>
    SPDX-License-Identifier: Apache-2.0
"""

import matplotlib.pyplot as plt
import numpy as np

print(plt.get_backend())

# use Qt6 as backend
plt.switch_backend('qtagg')

x = np.linspace(0, 2 * np.pi, 200)
y = np.sin(x)

fig, ax = plt.subplots()
ax.plot(x, y)
plt.show()
