package com.example.goodbyelamb;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.EditText;

public class StartScreenActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_start_screen);
		//TODO take away    	
	    Log.w("myDebugging", "start screen onCreate Sucseed");
	    
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.start_screen, menu);
		return true;
	}
	
	/** Called when the user clicks the CountUsInButton	 
	 *  Starts the CountingActivity  
	 **/
	public void startCountUsIn(View view) {
		Intent intent = new Intent(this, CountingActivity.class);
		startActivity(intent);
	}

}
