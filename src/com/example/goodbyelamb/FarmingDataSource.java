package com.example.goodbyelamb;


import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

/**
 * This class handle all communication with the datbase.
 * FarmingDataSource return java-objects representing the data asked for in
 * the database. 
 * @author ragnhild
 *
 */
public class FarmingDataSource {
	private DataBaseHelper dbHelper;

	public FarmingDataSource(Context context) {
		dbHelper = new DataBaseHelper(context);
	}

	public void open() throws SQLException {
	
		 try {
			 
			 dbHelper.createDataBase();
			  
			 } catch (IOException ioe) {
			  
			 throw new Error("Unable to create database");
			  
			 }
			  
			 try {
			  
			 dbHelper.openDataBase();
			  
			 }catch(SQLException sqle){
			  
			 throw sqle;
			  
			 }
			 close();
			 
			 try {
				  
				 dbHelper.openDataBase();
				 
				 Log.w("myDebugging", "opening second time");
			     
				  
				 }catch(SQLException sqle){
				  
				 throw sqle;
				  
				 }
//				 
//			 
	}
	

	public void close() {
		dbHelper.close();
	}

	/**
	 * Return an Arraylist with animal objects representing all animals
	 *  with the specified type in the databse
	 * @param animalType
	 * @return
	 */
	public ArrayList<Animal> getAllAnimalsByType(int animalType){
	ArrayList<Animal> animals = dbHelper.getAllAnimalsByType(animalType);
	
	return animals;
	
	}
	/**
	 * Saves the specified animals counting status
	 * 
	 */
	public void saveAnimalsCountingStatus(Animal animal){
		dbHelper.saveAnimalCountingStatus(animal);
	}
	/**
	 * Change all present animals counting status to uncounted
	 */
	public void markAnimalsToUncounted(){
		dbHelper.changeAllAnimalToUncounted();
		
	}
	

}
