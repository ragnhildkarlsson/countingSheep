package com.example.goodbyelamb;


public class AnimalType {
	private int typeID;
	private String type;
	
	public AnimalType(int id, String type){
		typeID = id;
		this.type = type; 
	}
	
	public String getType(){
		return type;
	}
	
	public int getTypeId(){
		return typeID;
	}
	
	

}
