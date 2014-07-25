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
