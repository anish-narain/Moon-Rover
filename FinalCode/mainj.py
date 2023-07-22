import requests
import pygame
from pygame.locals import *
from time import time

ip = "192.168.92.41"

pygame.init()
clock = pygame.time.Clock()
georgiou=pygame.image.load("georgiou.jpg")
georgiou=pygame.transform.scale(georgiou,(500,500))
screen = pygame.display.set_mode((500, 500))
pygame.display.set_caption("EEE Rover")
font1= pygame.font.SysFont("Comic Sans MS", 25)
font = pygame.font.SysFont("Comic Sans MS", 30)
rock = ""
BLACK=(0,0,0)
RED=(255,0,0)
arrow_colour_r=BLACK
arrow_colour_l=BLACK
arrow_colour_f=BLACK
arrow_colour_b=BLACK
running = True
while running:

    clock.tick(60)
    fps = clock.get_fps()
    pygame.display.set_caption(str(int(fps)))
    screen.fill((255, 255, 255))
    screen.blit(georgiou,(0,0))
    t = font.render(rock, False, (0, 0, 0))
    screen.blit(t, (5, 5))
    right = pygame.draw.polygon(screen, arrow_colour_r, ((320, 230), (320, 280), (450, 280), (450, 305), (490, 255), (450, 205), (450, 230)))
    left = pygame.draw.polygon(screen, arrow_colour_l, ((10, 255), (50, 305), (50, 280), (180, 280), (180, 230), (50, 230), (50, 205)))
    forward = pygame.draw.polygon(screen, arrow_colour_f, ((250, 40), (200, 80), (225, 80), (225, 210), (275, 210), (275, 80), (300, 80)))
    backward = pygame.draw.polygon(screen, arrow_colour_b, ((225, 300), (225, 430), (200, 430), (250, 470), (300, 430), (275, 430), (275, 300)))
    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.KEYDOWN:

            if event.key == K_w:
                arrow_colour_f=RED
                pygame.draw.polygon(screen, arrow_colour_f, ((250, 40), (200, 80), (225, 80), (225, 210), (275, 210), (275, 80), (300, 80)))
                pygame.display.update()
                r = requests.get(f"http://{ip}?State=F")
            elif event.key == K_s:
                arrow_colour_b=RED
                pygame.draw.polygon(screen, arrow_colour_b, ((225, 300), (225, 430), (200, 430), (250, 470), (300, 430), (275, 430), (275, 300)))
                pygame.display.update()
                r = requests.get(f"http://{ip}?State=B")
            elif event.key == K_a:
                arrow_colour_l=RED
                pygame.draw.polygon(screen, arrow_colour_l, ((10, 255), (50, 305), (50, 280), (180, 280), (180, 230), (50, 230), (50, 205)))
                pygame.display.update()
                r = requests.get(f"http://{ip}?State=L")
            elif event.key == K_d:
                arrow_colour_r=RED
                pygame.draw.polygon(screen, arrow_colour_r, ((320, 230), (320, 280), (450, 280), (450, 305), (490, 255), (450, 205), (450, 230)))
                pygame.display.update()
                r = requests.get(f"http://{ip}?State=R")
            elif event.key == K_SPACE:
                t = font.render(rock, False, (255, 0, 0)) # Displays red text whilst scanning
                screen.blit(t, (5, 5))
                pygame.display.update()
                r = requests.get(f"http://{ip}?State=SC")
                rock = r.content

        elif event.type == pygame.KEYUP:
            arrow_colour_r=BLACK
            arrow_colour_l=BLACK
            arrow_colour_f=BLACK
            arrow_colour_b=BLACK
            r = requests.get(f"http://{ip}?State=S")

    pygame.display.update()

pygame.quit()
