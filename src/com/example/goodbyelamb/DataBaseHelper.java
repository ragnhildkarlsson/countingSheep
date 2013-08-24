package com.example.goodbyelamb;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import android.R.string;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

    public class DataBaseHelper extends SQLiteOpenHelper{
    	
    private final String tag = "DataBaseHelper" ;
     
    //The Android's default system path of your application database.
    private static String DB_PATH = "/data/data/com.example.goodbyelamb/databases/";
     
    private static String DB_NAME = "farmDataTest";
     
    private SQLiteDatabase myDataBase;
     
    private final Context myContext;
    
    private static final int DATABASE_VERSION = 1;
    
    //The name of the tables and there columns
    
    //Same for all tabled
    public static final String COLUMN_ID = "_id";
    		
    public static final String TABLE_ANIMAL = "animal";
    public static final String COLUMN_TID = "tid";
    public static final String COLUMN_COUNTED = "counted";
 ;
    
    public static final String TABLE_ANIMAL_TYPE = "animaltype";
    public static final String COLUMN_TYPE = "type";
;
    /**
      * Constructor
      * Takes and keeps a reference of the passed context in order to access to the application assets and resources.
      * @param context
      */
    public DataBaseHelper(Context context) {
     
    super(context, DB_NAME, null, 1);
    this.myContext = context;
    }	
     
    /**
      * Creates a empty database on the system and rewrites it with your own database.
      * */
    public void createDataBase() throws IOException{
    
    	//TODO take away
    	
    Log.w("myDebugging", "steped in to create Database");
    
    	boolean dbExist = checkDataBase();
     
    if(dbExist){
    //do nothing - database already exist
    	//TODO take away
	    Log.w("myDebugging", "thinks the databse allready there");
    }else{
    	
    	File f = new File(DB_PATH);
    	if (!f.exists()) {
    	Log.w("myDebugging", "file din not existed");
    	     	
    	f.mkdir();
    	}	
    	//TODO take away
	    Log.w("myDebugging", "copying the database will start");
     
    //By calling this method and empty database will be created into the default system path
    //of your application so we are gonna be able to overwrite that database with our database.
    this.getReadableDatabase();
     
    try {
     
    copyDataBase();
    Log.w("myDebugging", "copying sucseed");
     
    } catch (IOException e) {
    Log.w("myDebugging", "error copying database");
        
    throw new Error("Error copying database");
     
    }
    }
     
    }
     
    /**
      * Check if the database already exist to avoid re-copying the file each time you open the application.
      * @return true if it exists, false if it doesn't
      */
    private boolean checkDataBase(){
     
    SQLiteDatabase checkDB = null;
     
    try{
    String myPath = DB_PATH + DB_NAME;
    checkDB = SQLiteDatabase.openDatabase(myPath, null, SQLiteDatabase.OPEN_READWRITE);
     
    }catch(SQLiteException e){
     
    //database does't exist yet.
     
    }
     
    if(checkDB != null){
     
    checkDB.close();
     
    }
     
    return checkDB != null ? true : false;
    }
     
    /**
      * Copies your database from your local assets-folder to the just created empty database in the
      * system folder, from where it can be accessed and handled.
      * This is done by transfering bytestream.
      * */
    private void copyDataBase() throws IOException{
     
    //Open your local db as the input stream
    InputStream myInput = myContext.getAssets().open(DB_NAME);
     
    // Path to the just created empty db
    String outFileName = DB_PATH + DB_NAME;
     
    //Open the empty db as the output stream
    OutputStream myOutput = new FileOutputStream(outFileName);
     
    //transfer bytes from the inputfile to the outputfile
    byte[] buffer = new byte[1024];
    int length;
    while ((length = myInput.read(buffer))>0){
    myOutput.write(buffer, 0, length);
    }
     
    //Close the streams
    myOutput.flush();
    myOutput.close();
    myInput.close();
     
    }
     
    public void openDataBase() throws SQLException{
     
    //Open the database
    String myPath = DB_PATH + DB_NAME;
    myDataBase = SQLiteDatabase.openDatabase(myPath, null, SQLiteDatabase.OPEN_READWRITE);
     
    }
     
    @Override
    public synchronized void close() {
     
    if(myDataBase != null)
    myDataBase.close();
     
    super.close();
     
    }
     
    @Override
    public void onCreate(SQLiteDatabase db) {
    	
    }
     
    @Override
    //TODO It seems that onUpgrade Never will be called but if it happens this
    // version will not be able to handle that.
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        
    }
     
    
    // Add your public helper methods to access and get content from the database.
    // You could return cursors by doing "return myDataBase.query(....)" so it'd be easy
    // to you to create adapters for your views.

    /**
     * Mark all animals in the database as uncounted
     */
    
    public void changeAllAnimalToUncounted(){
    	ContentValues cv = new ContentValues();
    	Integer counted = 0;   	
    	cv.put(DataBaseHelper.COLUMN_COUNTED, counted);   	
    	myDataBase.update(DataBaseHelper.TABLE_ANIMAL,cv,null,null);
    	Log.w(tag, "switched to uncounted");
        
    }
    
    /**
     * Save the specified animals couinting status in to the database 
     */
    
    public void saveAnimalCountingStatus(Animal animal){
    	Log.w(tag, "steped into savAnimalCountingStatus");
    	ContentValues cv = new ContentValues();
    	Integer counted;
    	if(animal.getCounted()){
    		counted = 1;
    	}
    	else{
    		counted = 0;
    	}
    	
    	cv.put(DataBaseHelper.COLUMN_COUNTED, counted);
    	Log.w(tag, "prepared data for"+animal.getID());
    	myDataBase.update(DataBaseHelper.TABLE_ANIMAL, cv, "_id "+ "=" + animal.getID(), null);
    	Log.w(tag, "executed update for animal"+animal.getID());
        
    }
    
    /**
	 * Return an Arraylist with animal objects representing all animals
	 *  with the specified type in the databse
	 * @param animalType
	 * @return
	 */
	public ArrayList<Animal> getAllAnimalsByType(int animalType){   
	ArrayList<Animal> animals = new ArrayList<Animal>();		
	String whereString = DataBaseHelper.COLUMN_TID + "=" + animalType;
	Log.w("myDebugging", "steped in to get all animalsbytype");
	
    Cursor cursor = myDataBase.query(DataBaseHelper.TABLE_ANIMAL,
        null, whereString, null, null, null, null);
    Log.w("myDebugging", "querry for all sheep exequted");
     
    cursor.moveToFirst();
    while (!cursor.isAfterLast()) {
      Animal animal = cursorToAnimal(cursor);
      animals.add(animal);
      cursor.moveToNext();
    }
    // Make sure to close the cursor
    cursor.close();
    return animals;
  }

	
private Animal cursorToAnimal(Cursor cursor){
    Animal animal = new Animal(cursor.getInt(cursor.getColumnIndexOrThrow(DataBaseHelper.COLUMN_ID)));
    int counted = cursor.getInt(cursor.getColumnIndexOrThrow(DataBaseHelper.COLUMN_COUNTED));
    if(counted>0){
    	animal.setCounted(true);				
    }
    else{
    	animal.setCounted(false);
    }
    return animal;
}

}