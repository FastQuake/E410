require "vector"
PlayerShip = {}
PlayerShip.__index = PlayerShip

function PlayerShip.create()
	ship = {}
	setmetatable(ship, PlayerShip)
	ship.ship = GO.loadIQM("ship.iqm","ship")
	ship.ship:setTriangleBody()
	ship.ship:setMass(0)

	ship.chair = GO.loadIQM("cube.iqm","chair")
	ship.chair:setBoxBody()
	ship.chair:setMass(0)
	ship.chair:setPos(23,4,0)
	ship.chair:setScale(2.8,6,2.5)
	ship.chair:setVisible(false)

	ship.vel = Vector.create(0,0,0)
	ship.rot = Vector.create(0,0,0)
	ship.deltaRot = Vector.create(0,0,0)
	return ship
end

function PlayerShip:relativeMove(objectList)
	for k,v in pairs(objectList) do
		local pos = Vector.create(v:getPos())
		--[[local fwd = Vector.create(0,0,0)
		fwd.x = pos:mag()*math.sin(math.rad(self.rot.x))*math.cos(math.rad(self.rot.y))
		fwd.y = pos:mag()*math.sin(math.rad(self.rot.x))*math.sin(math.rad(self.rot.y))
		fwd.z = pos:mag()*math.cos(math.rad(self.rot.x))
		fwd = fwd:normalize()
		local right = Vector.cross(fwd,Vector.create(0,1,0))
		right = right:normalize()
		local s = axisToEuler(right,self.rot.x)
		s = Vector.scalarMul(0.5, s)
		s.y = s.y + self.rot.y/2]]--
		--[[local rot = Vector.create(mathMagic(self.rot:get()))
		rot = Vector.scalarMul(0.5,rot);
		local x = rot.x
		local z = rot.z
		rot.x = z
		rot.z = x]]--
		--x,y,z = pos:get()
		--dx,dy,dz = self.deltaRot:get()
		--pos = Vector.create(matRot(x,y,z,dz,dy,-dx))
		--pos = pos:rotate(-self.deltaRot.x, right)
		local a = Vector.create(v:getLookat())
		local b = Vector.cross(a,Vector.create(0,1,0))
		pos = pos:rotate(-self.deltaRot.x, Vector.create(0,0,1))
		pos = pos:rotate(self.deltaRot.y, Vector.create(0,1,0))
		--pos = Vector.scalarMul(-delta, self.vel) + pos
		v:setPos(pos:get())
		v:setVelocity(Vector.scalarMul(-1,self.vel):get())
		v:setRot(Vector.scalarMul(0.5,self.rot):get())
		--self.ship:setRot(self.rot:get())
	end
	self.deltaRot = Vector.create(0,0,0)
end
