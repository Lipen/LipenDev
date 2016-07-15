import numpy as np
from matplotlib import pyplot as plt


def convection_linear_1d(nx, nts=None):
    # du/dy + c*du/dx = 0
    # u^(n+1)_i = u^(n)_i - c*dt/dx*(u^(n)_i - u^(n)_(i-1))
    # u[i]  --  u^(n+1)_i
    # un[i] --  u^(n)_i
    if nts is None:
        nts = {5: 'c', 10: 'r', 20: 'g', 30: 'm', 50: 'k'}
    dx = 2 / (nx - 1)
    sigma = 0.5
    dt = sigma * dx
    c = 1

    u = np.ones(nx)
    u[int(.5 / dx): int(1 / dx + 1)] = 2
    plt.subplot(311)
    plt.plot(np.linspace(0, 2, nx), u, 'b')

    for nt, color in nts.items():
        v = u.copy()

        for n in range(nt):
            vn = v.copy()  # 'previous'
            for i in range(1, nx):
                v[i] = vn[i] - c * dt / dx * (vn[i] - vn[i - 1])

        plt.plot(np.linspace(0, 2, nx), v, color)

    plt.xlim(0, 2.1)
    plt.ylim(1, 2.1)


def convection_nonlinear_1d(nx, nts=None):
    # du/dy + u*du/dx = 0
    # u^(n+1)_i = u^(n)_i - u^(n)_i*dt/dx*(u^(n)_i - u^(n)_(i-1))
    if nts is None:
        nts = {5: 'c', 10: 'r', 20: 'g', 30: 'm', 50: 'k'}
    dx = 2 / (nx - 1)
    sigma = 0.5
    dt = sigma * dx

    u = np.sin(np.linspace(0, 3.14, num=nx))
    plt.subplot(312)
    plt.plot(np.linspace(0, 2, nx), u, 'b')

    for nt, color in nts.items():
        v = u.copy()

        for n in range(nt):
            vn = v.copy()  # 'previous'
            for i in range(1, nx):
                v[i] = vn[i] - vn[i] * dt / dx * (vn[i] - vn[i - 1])

        plt.plot(np.linspace(0, 2, nx), v, color)

    plt.xlim(0, 2.1)
    plt.ylim(0, 1.1)


def diffusion_1d(nx, nts=None):
    if nts is None:
        nts = {50: 'r', 20: 'g'}
    dx = 2 / (nx - 1)
    nu = 0.3
    sigma = 0.2
    dt = sigma * dx**2 / nu

    u = np.ones(nx)
    u[int(.5 / dx): int(1 / dx + 1)] = 2
    plt.subplot(313)
    plt.plot(np.linspace(0, 2, nx), u, 'b')

    for nt, color in nts.items():
        v = u.copy()

        for n in range(nt):
            vn = v.copy()  # 'previous'
            for i in range(1, nx - 1):
                v[i] = vn[i] + nu * dt / dx / dx * (vn[i + 1] - 2 * vn[i] + vn[i - 1])

        plt.plot(np.linspace(0, 2, nx), v, color)

    plt.xlim(0, 2.1)
    plt.ylim(1, 2.1)


convection_linear_1d(41)
convection_nonlinear_1d(41)
diffusion_1d(20)

plt.show()
