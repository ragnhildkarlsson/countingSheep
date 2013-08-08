package com.example.goodbyelamb;

import java.util.ArrayList;
import java.util.List;

import android.os.Bundle;
import android.app.ListActivity;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.ArrayAdapter;

public class CountingActivity extends ListActivity {
	
	//For Debugging
	private String tag = "CountingActivity";
	
	private FarmingDataSource datasource;

	//TODO HARDCODED animal type just for test
	private int animaltype_sheep = 1;
	private int animaltype_lamb =2;
	private InteractiveBaseAdapter presentArrayAdapter;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_counting);

		
		//TODO take away

		Log.w(tag, "before creating datasource");

		datasource = new FarmingDataSource(this);
		//TODO take away

		Log.w(tag, "creating datsourse sucseed");

		datasource.open();

		Log.w(tag, "opening datsourse sucseed");
		
		ArrayList<Animal> values = datasource.getAllAnimalsByType(animaltype_sheep);
		ArrayList<Animal> lambs = datasource.getAllAnimalsByType(animaltype_lamb);
		values.addAll(lambs);
		
		
		//presentAnimals = values;
		Log.w(tag, "returning animals sucseed");

		presentArrayAdapter =  new InteractiveBaseAdapter(this,
		        values);
		Log.w(tag, "created second Interactive ArrayAdapter");
	    setListAdapter(presentArrayAdapter);
	    Log.w(tag, "set second Interactive ArrayAdapter as Adapter");
	    getListView().setFastScrollEnabled(true);

	
	}


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.counting, menu);
		return true;
	}
	
	/**
	 * Called when the user click show_all_button
	 * Change the content in the list to show all animals   
	 */
	public void showAll(View view) {
	presentArrayAdapter.showAll();
	}
	
	/**
	 * Called when the user click hide_counted_button 
	 * Change the content in the list to show only the uncounted animals
	 */
	public void hideCounted(View view){
		presentArrayAdapter.hideAllCounted();
	}
	
	
	/**
	 * Called when the user click the counted all button.
	 * Starts the counted all activity
	 * @param view
	 */

	public void exit(View view) {
		Intent intent = new Intent(this, CountedAllActivity.class);
		startActivity(intent);
	}

	@Override
	protected void onResume() {
		datasource.open();
		super.onResume();
	}
	

	@Override
	protected void onPause() {
		datasource.close();
		super.onPause();
	}
	

}
