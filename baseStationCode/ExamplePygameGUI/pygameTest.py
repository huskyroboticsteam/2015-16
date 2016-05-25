import io
import sys, pygame
pygame.init()
pygame.joystick.init()
joysticks = [pygame.joystick.Joystick(x) for x in range(pygame.joystick.get_count())]

#https://www.pygame.org/docs/ref/joystick.html

clock = pygame.time.Clock()
fps = 60

size = width,height = 1280,720
black = 0,0,0

screen = pygame.display.set_mode(size)
background = pygame.image.load("MarsDesertResearchStation.png")
ball = pygame.image.load("ball.png")

x,y = 0,0

while True:
    clock.tick(fps)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:sys.exit()
    screen.fill(black)
    screen.blit(background,(0,0))
    screen.blit(ball,(x,y))
    pygame.display.flip()

    joysticks[0].init()

    axisx = joysticks[0].get_axis(0)
    axisy = joysticks[0].get_axis(1)

    print pygame.joystick.get_count()

    x += axisx
    y += axisy

    pygame.event.pump()