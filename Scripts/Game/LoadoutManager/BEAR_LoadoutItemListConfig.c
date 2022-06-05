[BaseContainerProps(configRoot:true)]
class BEAR_LoadoutItemListConfig
{
	[Attribute("", UIWidgets.EditBox)]
	protected string Name;
	
	[Attribute(params: "et")]
	protected ResourceName RifleResource;
	
	[Attribute(params: "et")]
	protected ResourceName HelmetResource;
	
	[Attribute(params: "et")]
	protected ResourceName ShirtResource;
	
	[Attribute(params: "et")]
	protected ResourceName VestResource;
	
	[Attribute(params: "et")]
	protected ResourceName BackpackResource;
	
	[Attribute(params: "et")]
	protected ResourceName PantsResource;
	
	[Attribute(params: "et")]
	protected ResourceName ShoesResource;
	
	
	[Attribute(params: "et")]
	protected ResourceName CompassResource;
	[Attribute(params: "et")]
	protected ResourceName BinocularsResource;
	[Attribute(params: "et")]
	protected ResourceName MapResource;
	
	
	[Attribute(params: "et")]
	protected ref array<ref ResourceName> Other;
	
	void SCR_LoadoutItemListConfig()
	{
		
	}
	
	Resource GetRifleResource()
	{
		return Resource.Load(RifleResource);
	}
	
	Resource GetHelmetResource() { return Resource.Load(HelmetResource);}
	Resource GetShirtResource() { return Resource.Load(ShirtResource);}
	Resource GetVestResource() { return Resource.Load(VestResource);}
	Resource GetBackpackResource() { return Resource.Load(BackpackResource);}
	Resource GetPantsResource() { return Resource.Load(PantsResource);}
	Resource GetShoesResource() { return Resource.Load(ShoesResource);}
	
	Resource GetCompassResource() { return Resource.Load(CompassResource);}
	Resource GetBinocularsResource() { return Resource.Load(BinocularsResource);}
	Resource GetMapResource() { return Resource.Load(MapResource);}
	
	array<ref ResourceName> GetOther() {return Other;}
	
	string GetName(){return Name;}
}