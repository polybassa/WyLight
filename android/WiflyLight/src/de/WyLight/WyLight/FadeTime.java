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

public class FadeTime {

	/**
	 * assure MAPPING.length >= 1
	 */
	private static final int MAPPING[] = { 100, 200, 300, 400, 500, 600, 700,
			800, 900, 1000, 1100, };

	public static int getMaxIndex() {
		return MAPPING.length - 1;
	}

	public static int indexToTmms(int index) {
		return MAPPING[Math.min(getMaxIndex(), Math.max(0, index))];
	}

	public static int timeToIndex(int tmms) {
		for (int i = 0; i < MAPPING.length; ++i) {
			if (tmms <= MAPPING[i]) {
				return i;
			}
		}
		return MAPPING.length - 1;
	}

	public static float timeToScaling(int tmms) {
		return 0.2f * (1 + timeToIndex(tmms));
	}
}
