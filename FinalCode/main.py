import requests
import pygame
from pygame.locals import *
from time import time

ip = "192.168.92.41"

pygame.init()
clock = pygame.time.Clock()

screen = pygame.display.set_mode((500, 50))
pygame.display.set_caption("EEE Rover")

font = pygame.font.SysFont("Comic Sans MS", 30)
rock = ""

running = True
while running:

    clock.tick(60)
    fps = clock.get_fps()
    pygame.display.set_caption(str(int(fps)))

    screen.fill((255, 255, 255))

    t = font.render(rock, False, (0, 0, 0))
    screen.blit(t, (5, 5))

    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.KEYDOWN:

            if event.key == K_w:
                r = requests.get(f"http://{ip}?State=F")

            elif event.key == K_s:
                r = requests.get(f"http://{ip}?State=B")

            elif event.key == K_a:
                r = requests.get(f"http://{ip}?State=L")

            elif event.key == K_d:
                r = requests.get(f"http://{ip}?State=R")

            elif event.key == K_SPACE:
                t = font.render(rock, False, (255, 0, 0)) # Displays red text whilst scanning
                screen.blit(t, (5, 5))
                pygame.display.update()
                r = requests.get(f"http://{ip}?State=SC")
                rock = r.content

        elif event.type == pygame.KEYUP:
            r = requests.get(f"http://{ip}?State=S")

    pygame.display.update()

pygame.quit()
