Peer = {}
Peer.__index = Peer

function Peer.create(modelName, tag)
	local p = {}
	setmetatable(p, Peer)
	p.id = peerID
	p.fwd = 0
	p.right = 0
	p.model = GO.loadIQM("guy.iqm","player".. peerID)
	--p.model:setBoxBody()
	p.model:setExtBoxBody(-0.5,0,-0.5,0.5,2.5,0.5)
	p.model:setActivation(true)
	p.model:setMass(100)
	p.model:setPos(0,10,0)
	p.model:lockAxis(0,0,0)
	peerID = peerID + 1
	return p
end

function Peer.getPeer(list, id)
	for k,v in pairs(list) do
		if v.id  == id then
			return v
		end
	end
	return nil
end

function Peer.getIDFromObject(list, obj)
	local id = obj:getID()
	for k,v in pairs(list) do
		if v.model:getID() == id then
			return v.id
		end
	end
	return nil
end

function getModelFromID(id)
	for k,v in ipairs(serverObjects) do
		if id == v:getID() then
			return v
		end
	end
	return nil
end
