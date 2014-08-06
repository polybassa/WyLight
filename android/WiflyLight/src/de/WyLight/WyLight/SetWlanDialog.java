package de.WyLight.WyLight;

import de.WyLight.WyLight.R;
import de.WyLight.WyLight.exception.FatalError;
import de.WyLight.WyLight.library.Endpoint;
import android.app.DialogFragment;
import android.os.Bundle;
import android.text.InputType;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;

public class SetWlanDialog extends DialogFragment {
	private Endpoint mRemote;
	private EditText mDeviceId;
	private EditText mPass;
	private EditText mSsid;
	private CheckBox mSoftAp;

	public SetWlanDialog() {
		//empty ...
	}

	public void setRemote(Endpoint remote) {
		mRemote = remote;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

		View v = inflater.inflate(R.layout.dialog_set_wlan, container);
		getDialog().setTitle(R.string.title_dialog_set_wlan);

		mDeviceId = (EditText) v.findViewById(R.id.editDeviceId);
		mPass = (EditText) v.findViewById(R.id.editPassphrase);
		mSsid = (EditText) v.findViewById(R.id.editSsid);
		mSoftAp = (CheckBox) v.findViewById(R.id.toggleSoftAp);

		CheckBox toggle = (CheckBox) v.findViewById(R.id.togglePassphrase);
		toggle.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

			public void onCheckedChanged(CompoundButton buttonView,
					boolean isChecked) {
				if (isChecked) {
					mPass.setInputType(InputType.TYPE_CLASS_TEXT
							| InputType.TYPE_TEXT_VARIATION_VISIBLE_PASSWORD);
				} else {
					mPass.setInputType(InputType.TYPE_CLASS_TEXT
							| InputType.TYPE_TEXT_VARIATION_PASSWORD);
				}
			}
		});

		Button save = (Button) v.findViewById(R.id.save);
		save.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {

				WiflyControl control = new WiflyControl();
				try {
					control.connect(mRemote);
					control.confSetWlan(mPass.getText().toString(), mSsid
							.getText().toString(), mDeviceId.getText()
							.toString(), mSoftAp.isChecked());
					control.disconnect();
					mRemote.setName(mDeviceId.getText().toString());
				} catch (FatalError e) {
					Toast.makeText(getDialog().getContext(), e.getMessage(),
							Toast.LENGTH_SHORT).show();
				}
				dismiss();
			}
		});

		Button cancel = (Button) v.findViewById(R.id.cancel);
		cancel.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				getDialog().cancel();
			}
		});
		return v;
	}

	@Override
	public void onStart() {
		super.onStart();
		WiflyControl control = new WiflyControl();
		try {
			control.connect(mRemote);
			mDeviceId.setText(control.confGetDeviceId());
			mPass.setText(control.confGetPassphrase());
			mSsid.setText(control.confGetSsid());
			mSoftAp.setChecked(control.confGetSoftAp());
			control.disconnect();
		} catch (FatalError e) {
			Toast.makeText(getDialog().getContext(), R.string.msg_connectionfailed,
					Toast.LENGTH_SHORT).show();
			dismiss();
		}

	}
}
