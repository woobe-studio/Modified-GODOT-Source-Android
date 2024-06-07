/**************************************************************************/
/*  GodotTextInputWrapper.java                                            */
/**************************************************************************/


package org.godotengine.godot.input;

import org.godotengine.godot.*;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class GodotTextInputWrapper implements TextWatcher, OnEditorActionListener {
	// ===========================================================
	// Constants
	// ===========================================================
	private static final String TAG = GodotTextInputWrapper.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================
	private final GodotView mView;
	private final GodotEditText mEdit;
	private String mOriginText;
	private boolean mHasSelection;

	// ===========================================================
	// Constructors
	// ===========================================================

	public GodotTextInputWrapper(final GodotView view, final GodotEditText edit) {
		this.mView = view;
		this.mEdit = edit;
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	private boolean isFullScreenEdit() {
		final TextView textField = this.mEdit;
		final InputMethodManager imm = (InputMethodManager)textField.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
		return imm.isFullscreenMode();
	}

	public void setOriginText(final String originText) {
		this.mOriginText = originText;
	}

	public void setSelection(boolean selection) {
		mHasSelection = selection;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void afterTextChanged(final Editable s) {
	}

	@Override
	public void beforeTextChanged(final CharSequence pCharSequence, final int start, final int count, final int after) {
		//Log.d(TAG, "beforeTextChanged(" + pCharSequence + ")start: " + start + ",count: " + count + ",after: " + after);

		for (int i = 0; i < count; ++i) {
			GodotLib.key(KeyEvent.KEYCODE_DEL, KeyEvent.KEYCODE_DEL, 0, true);
			GodotLib.key(KeyEvent.KEYCODE_DEL, KeyEvent.KEYCODE_DEL, 0, false);

			if (mHasSelection) {
				mHasSelection = false;
				break;
			}
		}
	}

	@Override
	public void onTextChanged(final CharSequence pCharSequence, final int start, final int before, final int count) {
		//Log.d(TAG, "onTextChanged(" + pCharSequence + ")start: " + start + ",count: " + count + ",before: " + before);

		final int[] newChars = new int[count];
		for (int i = start; i < start + count; ++i) {
			newChars[i - start] = pCharSequence.charAt(i);
		}
		for (int i = 0; i < count; ++i) {
			int key = newChars[i];
			if ((key == '\n') && !mEdit.isMultiline()) {
				// Return keys are handled through action events
				continue;
			}
			GodotLib.key(0, 0, key, true);
			GodotLib.key(0, 0, key, false);
		}
	}

	@Override
	public boolean onEditorAction(final TextView pTextView, final int pActionID, final KeyEvent pKeyEvent) {
		if (this.mEdit == pTextView && this.isFullScreenEdit() && pKeyEvent != null) {
			final String characters = pKeyEvent.getCharacters();
			if (characters != null) {
				for (int i = 0; i < characters.length(); i++) {
					final int ch = characters.codePointAt(i);
					GodotLib.key(0, 0, ch, true);
					GodotLib.key(0, 0, ch, false);
				}
			}
		}

		if (pActionID == EditorInfo.IME_ACTION_DONE) {
			// Enter key has been pressed
			mView.queueEvent(() -> {
				GodotLib.key(KeyEvent.KEYCODE_ENTER, KeyEvent.KEYCODE_ENTER, 0, true);
				GodotLib.key(KeyEvent.KEYCODE_ENTER, KeyEvent.KEYCODE_ENTER, 0, false);
			});
			mView.requestFocus();
			return true;
		}
		return false;
	}

	// ===========================================================
	// Methods
	// ===========================================================

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
