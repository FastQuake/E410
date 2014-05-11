require "vector"
Bullet = {}
Bullet.__index = Bullet

tSprites = {}

function Bullet.create(pos,stop)
	local b = {}
	setmetatable(b, Bullet)

	b.pos = pos
	b.spos = pos
	b.stop = stop
	b.dir = (stop-pos):normalize()
	b.sprite = GO.createSprite("glow.png")
	b.sprite:setPos(pos:get())

	return b
end

function timedSprite(texture, pos, sizex, sizey, time)
	local t = {}

	t.time = 0
	t.endtime = time
	t.sprite = GO.createSprite(texture)
	t.sprite:setFrameSize(sizex,sizey)
	t.sprite:animate(true)
	t.sprite:setPos(pos:get())
	return t
end
