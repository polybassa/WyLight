/**
                Copyright (C) 2012 - 2014 Patrick Bruenn.

    This file is part of WyLight.

    Wylight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    WiflyLight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WiflyLight.  If not, see <http://www.gnu.org/licenses/>. */

package de.WyLight.WyLight;

import de.WyLight.WyLight.library.Endpoint;

public class BroadcastReceiver {
	private native long create(String path);

	private native long getEndpoint(long pNative, long index);

	private native long getNextRemote(long pNative, long timeoutNanos);

	private native void release(long pNative);

	private long mNative = 0;

	public BroadcastReceiver(String recentFilename) {
		mNative = create(recentFilename);
	}

	public Endpoint getEndpoint(long index) {
		return new Endpoint(mNative, getEndpoint(mNative, index));
	}

	public final long getNative() {
		return mNative;
	}

	public Endpoint getNextRemote(long timeoutNanos) {
		return new Endpoint(mNative, getNextRemote(mNative, timeoutNanos));
	}

	public void release() {
		release(mNative);
	}
}
